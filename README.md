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
    
  - ./source openbmc-env  (Setup script)
    
  - Build the kernel :     ./bitbake linux-yocto-fsp2 

  - Build the example recipies :    ./bitbake hello or bitbake rootfsexample

  - Clean command :    bitbake -c clean core-image-minimal
  	
  - Build the bare minimum rootfile system and the linux image :    bitbake core-image-minimal

## Notes ##

 - The distribution used is poky and not phosphorous. 
 - OpenBMC kernel used for FSP2 is 4.10.

## OpenBMC kernel ##

 - Kernel Image uImage is created inside directory tmp/work/fsp2-poky-linux/linux-yocto-fsp2/4.10+gitAUTOINC+d5adbfcd5f-r0/image/boot
 - rename the zImage-4.10.0-rc7-yocto-standard-custom to bbqefsp2_vmlinux.img
 - scp to the SE /console/power

## FSP2 Device Tree ##

 - Device tree is created inside the directory tmp/deploy/images/fsp2/ with the name zImage-fsp2.dtb
 - rename the zImage-fsp2.dtb to bbqefsp2_dtb.img
 - scp to the SE /console/power
 
## Root File system ##

 - Rootfile system is created inside the directory tmp/deploy/images/fsp2/ with the name core-image-minimal-fsp2.cpio.gz 
 - To generate its uImage you need to use mkimage tool 
 - Mkimage command :  mkimage -A ppc -O linux -T ramdisk -C none -a 0x01400000 -d core-image-minimal-fsp2.cpio.gz rootfsimg.img
 - rename the rootfsimg.img to bbqefsp2_rootfs.img
 - scp to the SE /console/power
 
## Uboot Image ##

 - Its added inside openBMC_PSCN folder with the name bbqefsp2_uboot.img
 - scp to the SE /console/power   
 
        
## FLD drivers and ADALs##

 - The Drivers and ADAL code is IBM internal and not hosted in public Github repository.
   
 - To access it you need to create account in https://github.ibm.com/.
  
 - For IBM employess : login Credentials are your IBM intranet ID and password. For cloning any project you need to add your ssh keys to the IBM github account(https://github.ibm.com/).

 - The meta-fsp2-ibm-internal layer is automatically integrated in the openBMCFsp2 project inside the /meta-openbmc-bsp/meta-ibm layer when you run the setupscript ie openbmc-env

 - Build the FLD drivers recipies :    ./bitbake fsptrace

## New Recipie ##

- Create a new recipie inside the new layer. Command : yocto-layer create mylayer
         
- Add  the new layer in bb file inside build directory.

- build the recipie   

    Command : ./bitbake recipiename
    
- Check where the build recipie is created.
  
   ./cd build/temp/work/...
   
   
   
    
     





