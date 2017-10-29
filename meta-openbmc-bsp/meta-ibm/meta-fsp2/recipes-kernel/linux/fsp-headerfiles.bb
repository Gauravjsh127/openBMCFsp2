DESCRIPTION = " FSP linux Header files"
LICENSE = "IBM-LICENSE"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/IBM-LICENSE;md5=f7b4ffd4b82a8971026b3fccf3d426bc"

SRC_URI += "file://fsp \
            file://additional-header-files \
 	  "	
FILES_${PN} += "/${includedir}/linux/fsp" 	  	   

S = "${WORKDIR}"

do_install() {
       install -d ${D}${includedir} ${D}${includedir}/linux ${D}${includedir}/linux/fsp 
       install -m 0755 ${S}/fsp/reboot.h ${D}${includedir}/linux/fsp  	
       install -m 0755 ${S}/additional-header-files/*.h ${D}${includedir}  
}

