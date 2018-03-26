# OpenBMCFsp2 #

Proof of concept : Experimental build for adding FSP2 chipset support in openBMC

## Building ##

OpenBMCFSP2 uses Yocto/Open-Embedded for a build system, which supports an
out-of-tree build.  It is recommended that you create an empty directory
somewhere to hold the build.  This directory will get big.

On Ubuntu 164.04 the following packages are required to build the default target

    sudo apt-get install -y git build-essential libsdl1.2-dev texinfo gawk chrpath diffstat mkimage

## Build steps ##

  - ./git clone git@github.com:Gauravjsh127/openBMCFsp2.git

  - ./cd openBMCFsp2
    
  - ./source openbmc-ppc-env  (Setup script)
    
  - Clean command :    bitbake -c clean core-image-minimal
  	
  - Build the bare minimum rootfile system and the linux image :    bitbake core-image-minimal

