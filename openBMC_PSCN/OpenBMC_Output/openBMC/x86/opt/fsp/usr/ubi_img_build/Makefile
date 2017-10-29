#
# Makefile
#
#
DEPTH = ../../..
include $(DEPTH)/integration/Rules.mk

ifeq ($(HOST_ARCH), sh4)
PLATFORMS = evb_nor_sample evb_nor_half
else
PLATFORMS = sunray_nand_sample sunray_nand2Gb_sample sunray_nor_no_kernel
endif

#PLATFORMS = sunray_nor_sample sunray_nand_sample     \
	    sunray_nor_network sunray_nand_network   \
	    sunray_nor_tftp-nfs sunray_nand_tftp-nfs \
	    f128m128_nand_bootstrap zsunlight_nand   \
	    psunlight_nand cts_nor_sample            \
	    sunray_nand2Gb_sample

# Not yet supported:
#	crp_nor bamboo_nand

all: $(PLATFORMS)

.PHONY: $(PLATFORMS)
$(PLATFORMS):
	$(MAKE) -C $@ all


install: clean
	install -d $(INST_USR_X86)/ubi_img_build
	cp -a * $(INST_USR_X86)/ubi_img_build
	rm -rf  $(INST_USR_X86)/ubi_img_build/pdd


clean distclean:
	for p in $(PLATFORMS); do	\
		$(MAKE) -C $$p $@;	\
	done
	$(RM) *~ *.LOG core
