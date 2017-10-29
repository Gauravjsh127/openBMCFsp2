#
# Makefile
# EVB NOR
#
DEPTH = ../../../..

card=evb
flash_type=raw_nor
pdd_data=evb_nor
mkpfi_cfg=../mkpfi_cfg/f8_nor_evb_half.cfg
bootenv_txt=../bootenv/bootenv_evb_nor.txt

include ../Rules.mk

compl_nor_mif=$(compl).$(flashtype)$(pagesize).mif

all: $(compl_pfi) $(compl_img) $(compl_nor_mif)

# Manufacturing image
#
$(compl_nor_mif): $(compl_img)
	cp -v $< $(compl_nor_mif)

distclean:	_distclean
	$(RM) flash?
