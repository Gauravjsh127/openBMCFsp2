#
# Makefile
# Sunray NOR
#
DEPTH = ../../../..

card=sunray
pdd_data=sunray_nor
mkpfi_cfg=../mkpfi_cfg/f64_nor_sample.cfg
bootenv_txt=../bootenv/bootenv_flash.txt
#bootenv_txt=../bootenv/bootenv_special.txt

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


simics.flash: $(compl_img)
	dd if=$(compl_img) of=flash1 bs=1M count=32
	dd if=$(compl_img) of=flash2 bs=1M count=32 skip=32

distclean:	_distclean
	$(RM) flash?
