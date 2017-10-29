#
# Makefile
# Sunray NOR
#
DEPTH = ../../../..

card=sunray
pdd_data=sunray_nor
bootenv_txt=../bootenv/bootenv_network.txt
mkpfi_cfg=../mkpfi_cfg/f64_nor_network.cfg

include ../Rules.mk

MKIMAGE=mkimage

compl_nor_mif=$(compl).$(flashtype)$(pagesize).mif

all: $(compl_pfi) $(compl_img) $(compl_nor_mif) ubootscript.img

# Manufacturing image
#
$(compl_nor_mif): $(compl_img)
	cp $< $(compl_nor_mif)

# 124 kiB dummy data + 4 kiB NOR IPL
#
$(ipl_bin): $(ipl_crc32) dummy.bin
	cat dummy.bin $(ipl_crc32) > $@

ubootscript.img:	ubootscript
			$(MKIMAGE) -A ppc -O linux -T script -C none \
			-a 0 -e 0 -n "uboot script for network boot" \
			-d $< $@

initrd.gz:
	cp ../../buildrootfs/rootfs.sunray.ubi.initrd.gz initrd.gz


dummy.bin:
	dd if=/dev/zero of=$@ bs=1024 count=124

distclean:	_distclean
		$(RM) ubootscript.img
		$(RM) initrd.gz
