#
# Makefile
# Bamboo NAND Network
#
DEPTH = ../../../..

card=bamboo
pdd_data=bamboo_nand
mkpfi_cfg?=../mkpfi_cfg/f32_nand_bamboo.cfg
bootenv_txt=../bootenv/bootenv_bamboo.txt

include ../Rules.mk

compl_nand512_mif=$(compl).$(flashtype)$(pagesize).mif
compl_nand512_img=$(compl).$(flashtype)$(pagesize).img
compl_nand512_oob=$(compl).$(flashtype)$(pagesize).oob

all: $(compl_pfi) $(compl_nand512_mif) $(compl_nand512_oob)

# Binary data and out of band data (OOB)
#
$(compl_nand512_mif): $(compl_img)
	$(bin2nand) -p $(pagesize) -o $(compl_nand512_mif) $<

# Out of band data (OOB)
#
$(compl_nand512_oob):  $(compl_img)
	$(bin2nand) -j 32Mi -p $(pagesize) -o $(compl_nand512_img) \
		 -q $(compl_nand512_oob) $<

# 4 * 4 kiB NAND IPL = 16KiB
#
$(ipl_bin): $(ipl_crc32)
	cat $(ipl_crc32) $(ipl_crc32) $(ipl_crc32) $(ipl_crc32) > $@

distclean:	_distclean
