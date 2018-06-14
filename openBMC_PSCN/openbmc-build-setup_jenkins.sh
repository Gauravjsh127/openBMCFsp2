#!/bin/bash 

###############################################################################
#
# This build script is for running the OpenBMC builds as containers with the
# option of launching the containers with Docker or Kubernetes.
#
###############################################################################
#
# Variables used for Jenkins build job matrix:
#  target       = barreleye|palmetto|witherspoon|firestone|garrison|evb-ast2500
#                 zaius|romulus|qemu|fsp2
#  distro       = fedora|ubuntu|boesedev|boemcp
#  imgtag       = Varies by distro. latest|16.04|14.04|trusty|xenial; 23|24|25;8.1.6
#  obmcext      = Path of the OpenBMC repo directory used in creating a copy
#                 inside the container that is not mounted to external storage
#                 default directory location "${WORKSPACE}/openbmc"
#  obmcdir      = Path of the OpenBMC directory, where the build occurs inside
#                 the container cannot be placed on external storage default
#                 directory location "/tmp/openbmc"
#  sscdir       = Path of the BitBake shared-state cache directoy, will default
#                 to directory "/home/${USER}", used to speed up builds.
#  WORKSPACE    = Path of the workspace directory where some intermediate files
#                 and the images will be saved to.
#
# Optional Variables:
#  launch       = job|pod
#                 Can be left blank to launch via Docker if not using
#                 Kubernetes to launch the container.
#                 Job lets you keep a copy of job and container logs on the
#                 api, can be useful if not using Jenkins as you can run the
#                 job again via the api without needing this script.
#                 Pod launches a container which runs to completion without
#                 saving anything to the api when it completes.
#  imgname      = Defaults to a relatively long but descriptive name, can be
#                 changed or passed to give a specific name to created image.
#  http_proxy   = The HTTP address for the proxy server you wish to connect to.
#  BITBAKE_OPTS = Set to "-c populate_sdk" or whatever other bitbake options
#                 you'd like to pass into the build.
#  ppbimg       = Option for populating build image inside docker via docker image.
#
###############################################################################

# Trace bash processing. Set -e so when a step fails, we fail the build
set -xeo pipefail

# Default variables
target=${target:-fsp2}
distro=${distro:-boesedev}
imgtag=${imgtag:-8.3.5.1}
obmcdir=${obmcdir:-/tmp/openbmcFSP2}
sscdir=${sscdir:-${HOME}/workspace/}
rnd="openBMC"-${RANDOM}
WORKSPACE=${WORKSPACE:-${HOME}/workspace/${rnd}}
obmcext=${obmcext:-${WORKSPACE}/openbmc}
launch=${launch:-}
http_proxy=${http_proxy:-}
ppbimg=${ppbimg:-1}
PROXY=""
openbmcCommitID=${openbmcCommitID:-HEAD}
ibminternalCommitID=${ibminternalCommitID:-HEAD}
dockercmd=docker
# Determine the architecture
ARCH=$(uname -m)

if [[ "${JENKINS}" == yes ]];then
	echo "running inside jenkins"
	dockercmd=$DOCKERBUILD_DOCKER_CMD
fi

echo "dockercommand is "$dockercmd


#echo ${target}
#echo ${obmcdir}
# Determine the prefix of the Dockerfile's base image
case ${ARCH} in
  "ppc64le")
    DOCKER_BASE="ppc64le/"
    ;;
  "x86_64")
    DOCKER_BASE=""
    ;;
  *)
    echo "Unsupported system architecture(${ARCH}) found for docker image"
    exit 1
esac

openBMCVersion="1"
    
# Timestamp for job
echo "Build started, $(date)"

CurrentDate=$(date +%F_%H.%M.%S)

# If the obmcext directory doesn't exist clone it in
if [ ! -d ${obmcext} ]; then
      echo "Clone in openbmc master to ${obmcext}"
      git clone git@github.com:Gauravjsh127/openBMCFsp2.git ${obmcext}
    if [[ "${openbmcCommitID}" != HEAD ]];then
          cd ${obmcext}   
          git checkout ${openbmcCommitID}  
          cd -  
    fi
      cd ${obmcext}
      git tag releases/"openBMC"-${CurrentDate}
      git push --tags 
      cd - 
      git clone git@github.ibm.com:XXPETRI/meta-fsp2-ibm-internal.git ${obmcext}/meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal
    if [[ "${ibminternalCommitID}" != HEAD ]];then
          cd ${obmcext}/meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal
          git checkout ${ibminternalCommitID}
          cd -    
    fi
      cd ${obmcext}/meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal
      git tag releases/"openBMC-IBM"-${CurrentDate}
      git push --tags 
      cd -   
fi

# Work out what build target we should be running and set BitBake command
case ${target} in
  barreleye)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-rackspace/meta-barreleye/conf source oe-init-build-env"
    ;;
  palmetto)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-ibm/meta-palmetto/conf source oe-init-build-env"
    ;;
  witherspoon)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-ibm/meta-witherspoon/conf source oe-init-build-env"
    ;;
  firestone)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-ibm/meta-firestone/conf source oe-init-build-env"
    ;;
  garrison)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-ibm/meta-garrison/conf source oe-init-build-env"
    ;;
  evb-ast2500)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-evb/meta-evb-aspeed/meta-evb-ast2500/conf source oe-init-build-env"
    ;;
  zaius)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-ingrasys/meta-zaius/conf source oe-init-build-env"
    ;;
  romulus)
    BITBAKE_CMD="TEMPLATECONF=meta-openbmc-machines/meta-openpower/meta-ibm/meta-romulus/conf source oe-init-build-env"
    ;;
  fsp2)
    BITBAKE_CMD=""
    BITBAKE_CMD_PPC="source openbmc-ppc-docker-env"
    BITBAKE_CMD_X86="source openbmc-x86-env"
    ;;
  qemu)
    BITBAKE_CMD="source openbmc-env"
    ;;
  *)
    exit 1
    ;;
esac

# Configure Docker build
if [[ "${distro}" == fedora ]];then

  if [[ -n "${http_proxy}" ]]; then
    PROXY="RUN echo \"proxy=${http_proxy}\" >> /etc/dnf/dnf.conf"
  fi
  DOCKER_BASE_BOE="fedora"
  DOCKER_BASE_PSCN="fedora"
  Dockerfile=$(cat << EOF
  FROM ${DOCKER_BASE}${distro}:${imgtag}
  ${PROXY}
  # Set the locale
  RUN locale-gen en_US.UTF-8
  ENV LANG en_US.UTF-8
  ENV LANGUAGE en_US:en
  ENV LC_ALL en_US.UTF-8
  RUN dnf --refresh install -y \
      bzip2 \
      chrpath \
      cpio \
      diffstat \
      findutils \
      gcc \
      gcc-c++ \
      git \
      make \
      patch \
      perl-bignum \
      perl-Data-Dumper \
      perl-Thread-Queue \
      python-devel \
      python3-devel \
      SDL-devel \
      socat \
      subversion \
      tar \
      texinfo \
      wget \
      which \
      uboot-tools\
      libxerces-c-dev \
      expect\
      cmake \
      unzip \
      rpm
  RUN grep -q ${GROUPS} /etc/group || groupadd -g ${GROUPS} ${USER}
  RUN grep -q ${UID} /etc/passwd || useradd -d ${HOME} -m -u ${UID} -g ${GROUPS} ${USER}
  ENV HOME ${HOME}
  RUN /bin/bash
EOF
)
elif [[ "${distro}" == ubuntu ]]; then

  if [[ -n "${http_proxy}" ]]; then
    PROXY="RUN echo \"Acquire::http::Proxy \\"\"${http_proxy}/\\"\";\" > /etc/apt/apt.conf.d/000apt-cacher-ng-proxy"
  fi
  DOCKER_BASE_BOE="ubuntu"
  DOCKER_BASE_PSCN="ubuntu"
  Dockerfile=$(cat << EOF
  FROM ${DOCKER_BASE}${distro}:${imgtag}
  ${PROXY}
  ENV DEBIAN_FRONTEND noninteractive
  RUN apt-get update && apt-get install -yy \
      build-essential \
      chrpath \
      debianutils \
      diffstat \
      gawk \
      git \
      libdata-dumper-simple-perl \
      libsdl1.2-dev \
      libthread-queue-any-perl \
      locales \
      python \
      python3 \
      socat \
      subversion \
      texinfo \
      cpio \
      wget \
      u-boot-tools \
      libxerces-c-dev \
      expect \
      cmake \
      unzip \
      rpm
  # Set the locale
  RUN locale-gen en_US.UTF-8
  ENV LANG en_US.UTF-8
  ENV LANGUAGE en_US:en
  ENV LC_ALL en_US.UTF-8
  RUN grep -q ${GROUPS} /etc/group || groupadd -g ${GROUPS} ${USER}
  RUN grep -q ${UID} /etc/passwd || useradd -d ${HOME} -m -u ${UID} -g ${GROUPS} ${USER}
  ENV HOME ${HOME}
  RUN /bin/bash
EOF
)
elif [[ "${distro}" == boemcp ]]; then
  if [[ -n "${http_proxy}" ]]; then
    PROXY="RUN echo \"Acquire::http::Proxy \\"\"${http_proxy}/\\"\";\" > /etc/apt/apt.conf.d/000apt-cacher-ng-proxy"
  fi
  DOCKER_BASE_BOE="fwdocker.boeblingen.de.ibm.com:5000"
  if [[ "${JENKINS}" == yes ]];then
      DOCKER_BASE_BOE="fwdocker.boeblingen.de.ibm.com:5000"
  fi
  DOCKER_BASE_PSCN="fwdocker.boeblingen.de.ibm.com:5004"
  Dockerfile=$(cat << EOF
  FROM ${DOCKER_BASE_BOE}/${distro}:${imgtag}
  ${PROXY}
  ENV LANG en_US.UTF-8
  ENV LANGUAGE en_US:en
  ENV LC_ALL en_US.UTF-8
  RUN yum-config-manager --add-repo http://mirror.centos.org/centos/7/os/x86_64/
  RUN yum install -y --nogpgcheck yum-plugin-ovl
  RUN yum install -y --nogpgcheck texinfo chrpath texi2html
  RUN grep -q ${GROUPS} /etc/group || groupadd -g ${GROUPS} ${USER}
  RUN grep -q ${UID} /etc/passwd || useradd -d ${HOME} -m -u ${UID} -g ${GROUPS} ${USER}
  ENV HOME ${HOME}
  RUN /bin/bash
EOF
)
elif [[ "${distro}" == boesedev ]]; then
  if [[ -n "${http_proxy}" ]]; then
    PROXY="RUN echo \"Acquire::http::Proxy \\"\"${http_proxy}/\\"\";\" > /etc/apt/apt.conf.d/000apt-cacher-ng-proxy"
  fi
  DOCKER_BASE_BOE="fwdocker.boeblingen.de.ibm.com:5000"
  if [[ "${JENKINS}" == yes ]];then
      DOCKER_BASE_BOE="fwdocker.boeblingen.de.ibm.com:5004"
  fi
  DOCKER_BASE_PSCN="fwdocker.boeblingen.de.ibm.com:5004"
  Dockerfile=$(cat << EOF
  FROM ${DOCKER_BASE_BOE}/${distro}:${imgtag}
  ${PROXY}
  ENV LANG en_US.UTF-8
  ENV LANGUAGE en_US:en
  ENV LC_ALL en_US.UTF-8
  RUN yum-config-manager --add-repo http://mirror.centos.org/centos/7/os/x86_64/
  RUN yum install -y --nogpgcheck yum-plugin-ovl
  RUN yum install -y --nogpgcheck texinfo chrpath texi2html
  RUN grep -q ${GROUPS} /etc/group || groupadd -g ${GROUPS} ${USER}
  RUN grep -q ${UID} /etc/passwd || useradd -d ${HOME} -m -u ${UID} -g ${GROUPS} ${USER}
  ENV HOME ${HOME}
  RUN /bin/bash
EOF
)
elif [[ "${distro}" == boesebase ]]; then
  if [[ -n "${http_proxy}" ]]; then
    PROXY="RUN echo \"Acquire::http::Proxy \\"\"${http_proxy}/\\"\";\" > /etc/apt/apt.conf.d/000apt-cacher-ng-proxy"
  fi
  DOCKER_BASE_BOE="fwdocker.boeblingen.de.ibm.com:5000"
  if [[ "${JENKINS}" == yes ]];then
      DOCKER_BASE_BOE="fwdocker.boeblingen.de.ibm.com:5004"
  fi
  DOCKER_BASE_PSCN="fwdocker.boeblingen.de.ibm.com:5004"
  Dockerfile=$(cat << EOF
  FROM ${DOCKER_BASE_BOE}/${distro}:${imgtag}
  ${PROXY}
  ENV LANG en_US.UTF-8
  ENV LANGUAGE en_US:en
  ENV LC_ALL en_US.UTF-8
  RUN yum-config-manager --add-repo http://mirror.centos.org/centos/7/os/x86_64/
  RUN yum install -y --nogpgcheck yum-plugin-ovl
  RUN yum install -y --nogpgcheck texinfo chrpath texi2html
  RUN wget https://www.python.org/ftp/python/3.6.4/Python-3.6.4.tar.xz; tar xf Python-3.*; cd Python-3.*; ./configure; make; make install
  RUN grep -q ${GROUPS} /etc/group || groupadd -g ${GROUPS} ${USER}
  RUN grep -q ${UID} /etc/passwd || useradd -d ${HOME} -m -u ${UID} -g ${GROUPS} ${USER}
  ENV HOME ${HOME}
  RUN /bin/bash
EOF
)
fi

# Create the Docker run script
export PROXY_HOST=${http_proxy/#http*:\/\/}
export PROXY_HOST=${PROXY_HOST/%:[0-9]*}
export PROXY_PORT=${http_proxy/#http*:\/\/*:}

mkdir -p ${WORKSPACE}

cat > "${WORKSPACE}"/build.sh << EOF_SCRIPT
#!/bin/bash
set -xeo pipefail
# Use the mounted repo cache to make an internal repo not mounted externally
cp -R ${obmcext} ${obmcdir}
# Go into the OpenBMC directory (the openbmc script will put us in a build subdir)
cd ${obmcdir}
# Set up proxies
export ftp_proxy=${http_proxy}
export http_proxy=${http_proxy}
export https_proxy=${http_proxy}
mkdir -p ${WORKSPACE}/bin
# Configure proxies for BitBake
if [[ -n "${http_proxy}" ]]; then
  cat > ${WORKSPACE}/bin/git-proxy << \EOF_GIT
  #!/bin/bash
  # \$1 = hostname, \$2 = port
  PROXY=${PROXY_HOST}
  PROXY_PORT=${PROXY_PORT}
  exec socat STDIO PROXY:\${PROXY}:\${1}:\${2},proxyport=\${PROXY_PORT}
EOF_GIT
  chmod a+x ${WORKSPACE}/bin/git-proxy
  export PATH=${WORKSPACE}/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:${PATH}
  git config core.gitProxy git-proxy
  mkdir -p ~/.subversion
  cat > ~/.subversion/servers << EOF_SVN
  [global]
  http-proxy-host = ${PROXY_HOST}
  http-proxy-port = ${PROXY_PORT}
EOF_SVN
fi

#### Protoc ####### 
mkdir protoc_grpc
cd protoc_grpc
git clone https://github.com/google/protobuf.git
cd protobuf/
git reset --hard  fe773bfe97854c4be95b1b6e66281c0868194269
git submodule update --init --recursive
./autogen.sh
./configure
make
make check
make install
cd ..

#### grpc ####### 
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
wget https://github.com/grpc/grpc/archive/v1.8.5.tar.gz
tar -xvzf v1.8.5.tar.gz
cd grpc-1.8.5
make
cp bins/opt/* /usr/local/bin/
cd ..
cd ..

rm -rf protoc_grpc

## Fetch the cppcheck tool and build it inside the docker and install it
git clone https://github.com/danmar/cppcheck
cd cppcheck
sudo make install CFGDIR=/usr/share/cppcheck/cfg
cd .. 
rm -rf cppcheck

## Fetch the crash tool and build it inside the docker
git clone https://github.com/xxpetri/crash-ppc32-cross.git
cd crash-ppc32-cross/
cd rpms_32bit
sudo rpm --install --force *
export LD_LIBRARY_PATH=/usr/lib/
sudo ln -fs /usr/lib/libz.so.1 /usr/lib/libz.so
sudo ln -fs /usr/lib/libtinfo.so.5 /usr/lib/libtinfo.so
sudo ln -fs /usr/lib/libncurses.so.5 /usr/lib/libncurses.so
cd ..
make target=PPC
make target=PPC extensions
cd ..
 
#### Delete Any previous machine conf settings ####### 
rm -rf build/conf

#### Build the fsp2 PPC target  ####### 
${BITBAKE_CMD_PPC}
# Custom BitBake config settings
cat >> conf/local.conf << EOF_CONF
BB_NUMBER_THREADS = "$(nproc)"
PARALLEL_MAKE = "-j$(nproc)"
DL_DIR="${sscdir}/bitbake_downloads"

EOF_CONF
# Kick off a build
# To generate the core-image-minimal
echo "Generate the core-image-minimal"
bitbake core-image-minimal 
# To generate the SDK for the core-image-minimal if BITBAKE_OPTS passed as -c populate_sdk
echo "Generate the SDK for the core-image-minimal"
bitbake core-image-minimal -c populate_sdk

cd ../
#### Delete Any previous machine conf settings ####### 
rm -rf build/conf

#### Build the PSCN x86 target  ####### 
${BITBAKE_CMD_X86}
# Custom BitBake config settings
cat >> conf/local.conf << EOF_CONF
BB_NUMBER_THREADS = "$(nproc)"
PARALLEL_MAKE = "-j$(nproc)"
DL_DIR="${sscdir}/bitbake_downloads"
EOF_CONF
# Kick off a build
# To generate the core-image-minimal
echo "Generate the x86 application"
bitbake -c clean core-image-minimal-x86

bitbake core-image-minimal-x86 

## Extract core-image-minimal-fsp2.cpio.gz files inside tmp/deploy/images/fsp2 folder inside build directory
cd tmp/deploy/images/fsp2
mkdir rootfs
cp core-image-minimal-fsp2.cpio.gz rootfs/
cd rootfs
gzip -cd core-image-minimal-fsp2.cpio.gz | cpio -idmv

cp ../../../../../../meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal/meta-fsp2-apps/recipes-apps/additional-rootfs/grpc/usr/bin/* usr/bin/
cp -r ../../../../../../meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal/meta-fsp2-apps/recipes-apps/additional-rootfs/grpc/usr/include/* usr/include/
cp -r ../../../../../../meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal/meta-fsp2-apps/recipes-apps/additional-rootfs/grpc/usr/lib/* usr/lib/
cp -r ../../../../../../meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal/meta-fsp2-apps/recipes-apps/additional-rootfs/grpc/usr/share/* usr/share/
cp -r ../../../../../../meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal/meta-fsp2-apps/recipes-apps/additional-rootfs/grpc/usr/src/* usr/src/

cd ../../../../../

# Copy images out of internal obmcdir into workspace directory
cp -R ${obmcdir}/build/tmp/deploy ${WORKSPACE}/deploy/

### Extract core-image-minimal-pscnx86.cpio.gz files inside tmp/deploy/images/pscnX86 folder inside build directory
cd tmp/deploy/images/pscnx86
mkdir rootfs
cp core-image-minimal-x86-pscnx86.cpio.gz rootfs/
cd rootfs
gzip -cd core-image-minimal-x86-pscnx86.cpio.gz | cpio -idmv

mkdir usr/lib/crash/
mkdir usr/lib/crash/extensions

cp usr/lib64/libfld* usr/lib/
cp usr/lib64/libclib* usr/lib/
cp usr/lib64/libdbgx* usr/lib/
cp usr/lib64/libffs* usr/lib/
cp usr/lib64/libtrace* usr/lib/
rm -rf  l* usr/lib64* 
rm -rf  usr/lib/opkg
cd ../../../../../

## copying crash tools build inside docker image to pscnx86 rootfs folder
cp ../crash-ppc32-cross/crash tmp/deploy/images/pscnx86/rootfs/usr/bin/
cp ../crash-ppc32-cross/extensions/*.so tmp/deploy/images/pscnx86/rootfs/usr/lib/crash/extensions/

cp ../meta-openbmc-bsp/meta-ibm/meta-fsp2-ibm-internal/meta-fsp2-apps/recipes-apps/crashtool/crashtool-x86/ppc-linux-gdb tmp/deploy/images/pscnx86/rootfs/usr/bin/
cd ../../
./openbmcFSP2/build/tmp/deploy/sdk/openbmc-phosphor-glibc-x86_64-core-image-minimal-powerpc-toolchain-2.2.sh -y
mkdir openbmc_output
cd openbmc_output
mkdir fsp2-x86
mkdir fsp2-ppc
cd ..
cp -r openbmcFSP2/build/tmp/deploy/images/fsp2/rootfs  openbmc_output/fsp2-ppc
cp -r openbmcFSP2/build/tmp/deploy/images/pscnx86/rootfs  openbmc_output/fsp2-x86
rm -rf openbmcFSP2


chown -R 1000 /opt/openbmc*
chmod -R a+rx /opt/openbmc*

chown -R 1000 /tmp/openbmc_output/*
chmod -R a+rx /tmp/openbmc_output/*

EOF_SCRIPT

chmod a+x ${WORKSPACE}/build.sh

# Determine if the build container will be launched with Docker or Kubernetes
if [[ "${launch}" == "" ]]; then

if [[ "${distro}" == boesedev ]]; then
  # Give the Docker image a name based on the distro,tag,arch,and target
  imgname=${imgname:-${DOCKER_BASE_PSCN}/${distro}-${imgtag}:"openBMC_${openBMCVersion}_dev"}
elif [[ "${distro}" == boesebase ]]; then
  # Give the Docker image a name based on the distro,tag,arch,and target
  imgname=${imgname:-${DOCKER_BASE_PSCN}/${distro}-${imgtag}:"openBMC_${openBMCVersion}_dev"}
else
  # Give the Docker image a name based on the distro,tag,arch,and target
  imgname=${imgname:-openbmc/${distro}:${imgtag}-${target}-${ARCH}}
fi
  # Build the Docker image
  $dockercmd build -t ${imgname} - <<< "${Dockerfile}"

  # If obmcext or sscdir are ${HOME} or a subdirectory they will not be mounted
  mountobmcext="-v ""${obmcext}"":""${obmcext}"" "
  mountsscdir="-v ""${sscdir}"":""${sscdir}"" "
  if [[ "${obmcext}" = "${HOME}/"* || "${obmcext}" = "${HOME}" ]];then
    mountobmcext=""
  fi
  if [[ "${sscdir}" = "${HOME}/"* || "${sscdir}" = "${HOME}" ]];then
    mountsscdir=""
  fi

  if [[ ! -z ${ppbimg} ]]; then
    # Run the Docker container, execute the build.sh script
    $dockercmd run \
    --cap-add=sys_admin \
    --net=host \
    -e WORKSPACE=${WORKSPACE} \
    -w "${HOME}" \
    --volume="${HOME}":"${HOME}" \
    ${mountobmcext} \
    ${mountsscdir} \
    -t ${imgname} \
    ${WORKSPACE}/build.sh
    cnt_id=$($dockercmd ps -lq)
    $dockercmd commit ${cnt_id} ${imgname}-"openBMCBuild"-${rnd}
    echo "Docker image has created ${imgname}-openBMCBuild"-${rnd}
  else
    # Run the Docker container, execute the build.sh script
    $dockercmd run \
    --cap-add=sys_admin \
    --net=host \
    --rm=true \
    -e WORKSPACE=${WORKSPACE} \
    -w "${HOME}" \
    -v "${HOME}":"${HOME}" \
    ${mountobmcext} \
    ${mountsscdir} \
    -t ${imgname} \
    ${WORKSPACE}/build.sh
  fi

elif [[ "${launch}" == "job" || "${launch}" == "pod" ]]; then

  # Source and run the helper script to launch the pod or job
  . ./kubernetes/kubernetes-launch.sh OpenBMC-build true true

else
  echo "Launch Parameter is invalid"
fi

# Timestamp for build
echo "openBMCBuild completed, $(date)"


FinalDockerfile=$(cat << EOF
    FROM ${imgname}-openBMCBuild-${rnd}
    RUN cp /tmp/openbmcFSP2/openBMC_PSCN/exec-as.c /tmp/exec-as.c
    RUN gcc -o /usr/sbin/exec-as /tmp/exec-as.c && rm -f /tmp/exec-as.c 
    RUN cp /tmp/openbmcFSP2/openBMC_PSCN/entrypoint.sh /entrypoint.sh
    ENTRYPOINT ["/entrypoint.sh"]
    CMD ["/bin/bash"]
EOF
)

# Build the Docker image
$dockercmd build -t ${imgname}-${CurrentDate} - <<< "${FinalDockerfile}"

if [[ "${JENKINS}" == yes ]];then
        echo "running inside jenkins"
        echo ${imgname}-${CurrentDate} > DOCKERIMAGENAME.txt
fi
  
