#
# Makefile
# Sunray NOR
#
DEPTH = ../../../..

card=sunray
pdd_data=sunray_nor
bootenv_txt=../bootenv/bootenv_tftp-nfs.txt
mkpfi_cfg=../mkpfi_cfg/f64_nor_tftp-nfs.cfg

include ../Rules.mk

compl_nor_mif=$(compl).$(flashtype)$(pagesize).mif

all: $(compl_pfi) $(compl_img) $(compl_nor_mif)

# Manufacturing image
#
$(compl_nor_mif): $(compl_img)
	cp $< $(compl_nor_mif)

# 124 kiB dummy data + 4 kiB NOR IPL
#
$(ipl_bin): $(ipl_crc32) dummy.bin
	cat dummy.bin $(ipl_crc32) > $@

dummy.bin:
	dd if=/dev/zero of=$@ bs=1024 count=124

distclean:	_distclean
