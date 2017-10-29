# CRP NOR Network boot (load kernel and initrd from u-boot)
DEPTH = ../../../..

card      = crp
pdd_data  = crp_nor
cpu       = .fsp0
mkpfi_cfg = ../mkpfi_cfg/f64_nor_network.cfg

include ../Rules.mk

compl_nor_mif=$(compl).$(flashtype)$(pagesize).mif

MKIMAGE ?= mkimage



all: ubootscript.img initrd.gz $(compl_pfi) $(compl_img)


ubootscript.img: ubootscript.txt
	$(MKIMAGE) -A ppc -O linux -T script -C none \
		   -a 0 -e 0 -n "CRP NOR network boot uboot script" \
		   -d $< $@

initrd.gz:
	ln -s $(DEPTH)/images/initrd.gz.fsp0 initrd.gz


$(ipl_bin): $(ipl_crc32) dummy.bin
	cat dummy.bin $(ipl_crc32) > $@

dummy.bin:
	dd if=/dev/zero of=$@ bs=1024 count=124



distclean:	_distclean
		$(RM) ubootscript.img
		$(RM) initrd.gz
