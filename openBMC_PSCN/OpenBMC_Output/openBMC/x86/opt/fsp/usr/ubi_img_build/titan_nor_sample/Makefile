#
# Makefile
# Titan NOR sample
#
DEPTH = ../../../..

card=titan
pdd_data=titan_nor
cpu=.fsp0
mkpfi_cfg=../mkpfi_cfg/f32_nor_fsp0_sample.cfg
bootenv_txt=../bootenv/bootenv_fsp0.txt

rootfs=$(DEPTH)/images/sample.rootfs.fsp0.jffs2.img

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
