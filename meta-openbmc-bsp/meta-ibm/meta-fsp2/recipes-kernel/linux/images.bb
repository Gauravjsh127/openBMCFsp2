DESCRIPTION = " images folder"
LICENSE = "IBM-LICENSE"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/IBM-LICENSE;md5=f7b4ffd4b82a8971026b3fccf3d426bc"

SRC_URI += "file://images \
 	  "	
FILES_${PN} += "/images" 	  	   

S = "${WORKDIR}"

do_install() {
       install -d ${D}/images 
       install -m 0755 ${S}/images/* ${D}/images  	  
}

