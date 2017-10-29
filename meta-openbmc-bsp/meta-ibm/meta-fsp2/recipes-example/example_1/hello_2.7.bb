DESCRIPTION = "GNU Helloworld application" 
SECTION = "examples" 
LICENSE = "GPLv3+" 
LIC_FILES_CHKSUM = "file://COPYING;md5=d32239bcb673463ab874e80d47fae504" 

SRC_URI[md5sum] = "fc01b05c7f943d3c42124942a2a9bb3a"
SRC_URI[sha256sum] = "fd593b5bcf6d1bb6d7d1bb7eefdccdc0010cf2c4985ccb445ef490f768b927c0"



PR = "r0" 
SRC_URI = "${GNU_MIRROR}/hello/hello-${PV}.tar.gz" 
inherit autotools gettext 

