# OpenBMCFsp2 #

 To Build PSCN application via openBMC there are two ways:
   - Using Docker - No dependency on host system  
   - Using Host system
   
   Note: Currently the build environment is tested on ubuntu 16:04 for other Linux distribution it may or may not work.
   
## Using Docker ##   
   - Install docker in your local system
  
  - Run the script :  openBMC_PSCN/Docker-build-setup-fsp2.sh.
     This script execution will take appox 1 hour and will generate 3 images in your local system.For an iterative build it will take approx 20-25 minutes but for any iterative changes to be  merge in build push them in github/github.ibm.com first.
     		          
	  	REPOSITORY        	TAG                     IMAGE ID          CREATED          SIZE
		openbmc/ubuntu  latest-fsp2-x86_64-2328916236   9a4fd8484028      14 hours ago    4 GB       (used by PSCN)
		openbmc/ubuntu  latest-fsp2-x86_64              99b63e49b013      19 hours ago   751.4 MB
		ubuntu          latest                          dd6f76d9cc        12 days ago    122 MB

   - checkout PSCN code branching to team/openBMC (use the /openBMC_PSCN/gitclone_pscn_openbmc.sh script)

   - Change the image name inside dockerbuild-imagename file to openbmc/ubuntu:latest-fsp2-x86_64-2328916236   

   - Now run the docker script to build the PSCN code.[3 steps]
  		-	./dockerbuild fsp 
		-	./dockerbuild fsp:package_rpm
		-	./dockerbuild fsp:package_ffdc 

   - Switch to the PSCN output directory ie cd build/build_fsp2
   
   - scp the ram0 ,ram1, bbqgtrc.bin and ffdc tar files to SE directory(preferable to a temporary directory and use fix command to copy them inside /console/power). 
  	 
	For example : scp bbqefsp2_ram1.tar images/bbqefsp2_ram0.tar bbqgtrc.bin bbqefsp2_ffdc.tgz hmcmanager@9.152.150.97:/home/hmcmanager/tmp
  		 
   - run the setup application on SE i.e. /console/power/bbqmcffilesetup.se
   
   - Reboot the FSPs.

   
## Using Host system ##

  Generating the cross-toolchain via BitBake and kernel images ie vmlinux,dtb,rootfs and uboot in your host system and using it to build PSCN code. 
  Make sure your system has libxerces-c-dev,cmake and rpm packages installed.

  - Populate the code : ./git clone git@github.com:Gauravjsh127/openBMCFsp2.git
	  -	 ./cd openBMCFsp2
    
  - Run setup script. 
 	 -	./source openbmc-env 

  - Build kernel images ie vmlinux,dtb,rootfs and uboot.
        bitbake core-image-minimal 
  
  - Build the SDK 
  	-	bitbake core-image-minimal -c populate_sdk 
   	 
  - checkout PSCN code branching to team/openBMC (use the /openBMC_PSCN/gitclone_pscn_openbmc.sh script)
	-	cd scsn/
             
  - Update the openBMC project path in the cmake/fsp2_compiler.template file inside the PSCN project ( VARIABLE : openBMCprojectpath ).
      ie Update these lines: openBMCprojectpath=/home/xxpetri/code/openBMC/openBMCFsp2/openBMCFsp22
    
  - Update the openBMC project path in the cmake/fsp2_toolchain file inside the PSCN project
		 i.e. set(openBMC /home/xxpetri/code/openBMC/openBMCFsp2/openBMCFsp22)

  - Update the openBMC project path in the cmake/buildrootfs file inside the PSCN project
		 i.e.  $openBMCprojectpath="/home/xxpetri/code/openBMC/openBMCFsp2/openBMCFsp22/";
		 
  - Update the  PSCN project path inside the /cmake/Modules/SetupFldLinks.cmake file inside the PSCN project. 
      		ie Update these lines:
      		COMMAND ln -T -s -f -v /home/xxpetri/code/openBMC/PSCN/scsn/OpenBMC_Output/openBMC ${PSCN_BINARY_DIR}/openBMC WORKING_DIRECTORY ${PSCN_BINARY_DIR}
      
  - Update sym links to rootfs for x86 and ppc inside scsn/OpenBMC_Output/openBMC
	-	For ppc:
		ln -fs /home/xxpetri/code/openBMC/openBMCFsp2/openBMCFsp22/build/tmp/work/fsp2-openbmc-linux/core-image-minimal/1.0-r0/rootfs/ ppc
	
	-	For x86s point to AFS - do a klog to get it and use ln -fs command to point sym link  to it

  - Build the PSCN code 
  	-	./buildctl.cm fsp2
 
  - Switch to the PSCN output directory.
  	-	cd ../build_fsp2
   
   - scp the ram0 ,ram1, bbqgtrc.bin and ffdc tar files to SE directory(preferable to a temporary directory and use fix command to copy them inside /console/power). 
  	 
	For example : scp bbqefsp2_ram1.tar images/bbqefsp2_ram0.tar bbqgtrc.bin bbqefsp2_ffdc.tgz hmcmanager@9.152.150.97:/home/hmcmanager/tmp
  		 
  - run the setup application on SE 
  	-	/console/power/bbqmcffilesetup.se
   
  - Reboot the FSPs.
  
