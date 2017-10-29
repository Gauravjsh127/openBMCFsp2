#
# Makefile
# zSunlight NAND
#
DEPTH = ../../../..

card=zsunlight
pdd_data=zsunlight_nand
mkpfi_cfg?=../mkpfi_cfg/f128_nand_sample.cfg

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
	$(bin2nand) -j 128Mi -p $(pagesize) -o $(compl_nand2048_img) \
		 -q $(compl_nand2048_oob) $<

# 4 * 4 kiB NAND IPL + 112 kiB dummy data
#
$(ipl_bin): $(ipl_crc32) dummy.bin
	cat $(ipl_crc32) $(ipl_crc32) $(ipl_crc32) $(ipl_crc32) dummy.bin > $@

dummy.bin:
	dd if=/dev/zero of=$@ bs=1024 count=112

distclean:	_distclean
