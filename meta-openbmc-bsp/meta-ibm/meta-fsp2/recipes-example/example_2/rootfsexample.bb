DESCRIPTION = "Hello World 2 Program"
PR = "r0"
LICENSE = "CLOSED"
RM_WORK_EXCLUDE += "hello2"


SRC_URI = "file://example_code.c \
           file://example_file \
           file://example_folder "

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} ${WORKDIR}/example_code.c -o example2
}

do_install() {
    install -m 0755 -d ${D}${bindir} ${D}${docdir}/example2
    install -m 0777 ${S}/example2 ${D}${bindir}
    install -m 0644 ${WORKDIR}/example_file ${D}${bindir}/example_file
    cp -r ${WORKDIR}/example_folder ${D}${bindir}/

}
