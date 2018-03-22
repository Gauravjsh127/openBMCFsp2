DESCRIPTION = "An RPC library and framework"
AUTHOR = "Google Inc."
HOMEPAGE = "http://www.grpc.io"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57"

inherit autotools-brokensep pkgconfig

DEPENDS = "\
	protobuf \
	zlib \
	openssl \
	c-ares \
"

SRC_URI = "gitsm://github.com/grpc/grpc.git;branch=v1.8.x"
SRCREV = "6a68148fe9ed405cc1a48bdc05c34970f6cbee99"

PR = "r0"

S = "${WORKDIR}/git"

# Fix static library generation
AR += "rcs"

OUTDIR = "${S}/libs/opt/"

do_compile () {
	export LD="${CC}"
	oe_runmake prefix=${exec_prefix} shared_c static_c
}

do_install () {
	oe_runmake prefix=${D}${exec_prefix} install-headers_c

	oe_libinstall -C ${OUTDIR} -so libgpr ${D}${libdir}
	oe_libinstall -C ${OUTDIR} -so libgrpc ${D}${libdir}
	oe_libinstall -C ${OUTDIR} -so libgrpc_cronet ${D}${libdir}
	oe_libinstall -C ${OUTDIR} -so libgrpc_unsecure ${D}${libdir}

	install -d ${D}${libdir}/pkgconfig
	install -m 0644 ${OUTDIR}/pkgconfig/*.pc ${D}${libdir}/pkgconfig/
}

INSANE_SKIP_${PN} = "already-stripped rpaths"
