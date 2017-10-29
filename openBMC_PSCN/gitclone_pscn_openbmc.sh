#!/bin/bash

# Version - 20160920.

# log in to the Gerrit server (https://fwgerrit.boeblingen.de.ibm.com:8443/gerrit/)
# Press on the top right on your user id an then 'Settings'
# At the next page you find the 'Username' as a numerical number.
USERID="162873724"
INTRANETID="GJOSHI@de.ibm.com"

#example
#USERID="123456724"
#INTRANETID="abc.def@de.ibm.com"


WORKDIR=scsn
REPOSITORY=zfsp


#########################  local functions ###########################
function remove_git_credential
{
    # Password is stored in clear words (!) in .git-credentials !
    # Using option 'cache...' !
    CRED=~/.git-credentials
    if [ -e $CRED ]; then
        rm -f $CRED
    fi
}

###############################  MAIN #################################

remove_git_credential

git lfs init
git lfs version
git --version

git clone --no-checkout ssh://$USERID@fwgerrit.boeblingen.de.ibm.com:29418/$REPOSITORY $WORKDIR && \
scp -p -P 29418 $USERID@fwgerrit.boeblingen.de.ibm.com:hooks/commit-msg $WORKDIR/.git/hooks/
cd $WORKDIR

git config lfs.url https://fwgerrit.boeblingen.de.ibm.com/nexus/service/local/lfs/${REPOSITORY}_lfs
git config credential.helper store
#git config credential.helper 'cache --timeout=300'
git config --global credential."https://fwgerrit.boeblingen.de.ibm.com".username $INTRANETID
git config http.sslverify false

sleep 5

git checkout dev/master

remove_git_credential

cd scsn

git checkout -b team/openbmc/poc origin/team/openbmc/poc

git branch --set-upstream-to=origin/team/openbmc/poc
