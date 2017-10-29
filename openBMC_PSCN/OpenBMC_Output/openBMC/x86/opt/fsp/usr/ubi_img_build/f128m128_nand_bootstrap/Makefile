#
# Makefile
# Sunray NAND Bootstrap Image
#
DEPTH = ../../../..

card=sunray_mfg
pdd_data=f128m128_nand
bootenv_txt=../bootenv/bootenv_bootstrap.txt
mkpfi_cfg?=../mkpfi_cfg/f128_nand_bootstrap.cfg
rootfs=$(DEPTH)/images/sample.rootfs.mini.initrd.gz

include ../Rules.mk

compl_nand2048_mif=$(compl).$(flashtype)$(pagesize).mif
compl_nand2048_img=$(compl).$(flashtype)$(pagesize).img
compl_nand2048_oob=$(compl).$(flashtype)$(pagesize).oob

all: $(compl_pfi) $(compl_nand2048_mif) $(compl_nand2048_oob)


# Binary data and out of band data (OOB)
#
$(compl_nand2048_mif): $(compl_img)
	$(bin2nand) -p $(pagesize) -o $(compl_nand2048_mif) $<

# Out of band data (OOB)
#
$(compl_nand2048_oob):  $(compl_img)
	$(bin2nand) -j $(flashsize) -p $(pagesize) -o $(compl_nand2048_img) \
		 -q $(compl_nand2048_oob) $<

# 124 kiB dummy data + 4 kiB NOR IPL
#
$(ipl_bin): $(ipl_crc32) dummy.bin
	cat $(ipl_crc32) $(ipl_crc32) $(ipl_crc32) $(ipl_crc32) dummy.bin > $@

dummy.bin:
	dd if=/dev/zero of=$@ bs=1024 count=112

distclean:	clean _distclean
	$(RM) *.bin *.crc32
