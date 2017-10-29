# OpenBMCFsp2 #

PSCN application build via openBMC ADT

## ADT GENERATION ##

Generating the cross-toolchain via BitBake within an existing build directory inside the ./tmp/sysroots path.


## Populate the code ##

  - ./git clone git@github.com:Gauravjsh127/openBMCFsp2.git

  - ./cd openBMCFsp2
    
  - ./source openbmc-env  (Setup script)

## Build kernel booting images ie vmlinux and device tree ##

  - Build the Images :    bitbake -k core-image-minimal 
  

## Build SDK ##

  - Build the SDK :    bitbake core-image-minimal -c populate_sdk 
     

## Prequiste before Using openBMC SDK to build PSCN code
 
  - package name libxerces-c-dev on Host machine.
   	 

## Using openBMC SDK to build PSCN code ##
   
	- checkout PSCN code branching to team/openBMC (use the /openBMC_PSCN/gitclone_pscn_openbmc.sh script)
	
	- cd scsn/
             
    - Update the openBMC project path in the cmake/fsp2_compiler.template file inside the PSCN project ( VARIABLE : openBMCprojectpath ).
      ie Update these lines: openBMCprojectpath=/home/gaurav/code/github/26th_july/openBMCFsp2
    
    - Update the openBMC project path in the cmake/fsp2_toolchain file inside the PSCN project for C, C++ and STRIP.
      ie Update these lines:
      C++   : set(PSCN_COMPILER " tracepp_zseries /home/gaurav/code/github/26th_july/openBMCFsp2/build/tmp/sysroots/x86_64-linux/usr/bin/powerpc-poky-linux/powerpc-poky-linux-g++ --sysroot=$SDKTARGETSYSROOT -lpthread -lresolv ")
      C     : set(PSCN_COMPILER " tracepp_zseries /home/gaurav/code/github/26th_july/openBMCFsp2/build/tmp/sysroots/x86_64-linux/usr/bin/powerpc-poky-linux/powerpc-poky-linux-gcc --sysroot=$SDKTARGETSYSROOT -lpthread -lresolv ")
      STRIP : set(CMAKE_STRIP /home/gaurav/code/github/26th_july/openBMCFsp2/build/tmp/sysroots/x86_64-linux/usr/bin/powerpc-poky-linux/powerpc-poky-linux-strip  CACHE PATH "Path to a program" FORCE)
    
    - Update the  PSCN project path inside the /cmake/Modules/SetupFldLinks.cmake file inside the PSCN project. 
      ie Update these lines:
      COMMAND ln -T -s -f -v /home/gaurav/BB/openBMC_pscn/scsn/OpenBMC_Output/openBMC ${PSCN_BINARY_DIR}/openBMC WORKING_DIRECTORY ${PSCN_BINARY_DIR}
  
    - Copy the openBMC output files and libraries generated via openBMC(bitbake -k core-image-minimal command) for the ppc and x86 architecture inside the /openBMC_Output folder inside the PSCN project.
      Note : Copy the ffs libraries from the package and not from rootfs to the openBMC_output 
      
      For example :
      OpenBMC file/libaries location : /home/gaurav/code/github/26th_july/openBMCFsp2/build/tmp/work/fsp2-poky-linux/core-image-minimal/1.0-r0/rootfs
      Copy to :
      PSCN project location          : /home/gaurav/BB/openBMC_pscn/scsn/OpenBMC_Output/openBMC/ppc
  
    - Build the PSCN code : ./buildctl.cm fsp2


## To Start PSCN application ##
    
   - Switch to the PSCN output directory ie cd ../build_fsp2
   
   - scp the ram0 and ram1 tar files to SE /console/power directory
  	 For example :  scp bbqefsp2_ram1.tar images/bbqefsp2_ram0.tar hmcmanager@9.152.171.108:/console/power
  		 
   - run the setup application on SE i.e. /console/power/bbqmcffilesetup.se
   
   - Reboot the FSPs.
  
   

   
   
    
     





