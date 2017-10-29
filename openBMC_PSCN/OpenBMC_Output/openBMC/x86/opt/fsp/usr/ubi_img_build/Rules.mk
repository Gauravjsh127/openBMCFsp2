#
# Rules.mk
#
# Adopt the toplevel Makefile to your needs. Try to keep this file
# untouched or add reasonable default parameters which you may
# overwrite in the card-specific Makefile.
#
# Author:	Frank Haverkamp <haverkam@de.ibm.com> 2005
#

#
# Some default values you might want to overwrite. Try it if you need
# it and add more if needed. Note that no real sanity checking is done
# on those values. If you do it wrong your card has no valid PDD data
# or else the mkpfi step will abend.

include $(DEPTH)/integration/Rules.mk

# Search our installed tools first (Changed because pfi2bin from MCP is buggy)
PATH := $(INSTALL_ROOT)/x86/usr/bin:$(PATH)
BINARIES = $(INST_BOOT)

sed		= sed
bin2nand	= bin2nand
ubigen		= ubigen
mkpfi		= mkpfi
pfi2bin		= pfi2bin

bootenv_complete?= bootenv_complete.txt
bootenv_bin	?= $(bootenv_complete:.txt=.bin)
bootenv_crc	?= $(bootenv_complete:.txt=.crc)
bootenv_txt	?= ../bootenv/bootenv_flash.txt
vmlinux_bin	?= cuImage.fsp1
rootfs_bin	?= rootfs.bin
spl_bin		?= u-boot.bin
ipl_bin		?= ipl.bin
ipl_crc32	?= ipl.crc32
pdd_txt		?= pdd
pdd_inc		 = pdd.inc
crtm_bin	?= crtm.bin
initramfs_bin	?= initramfs.bin


compl		?= $(card)_complete
compl_pfi	?= complete.pfi
compl_img	?= $(compl).img

flashtype = $(shell if test -r $(pdd_txt); then cat $(pdd_txt) | perl -e 'while (<>) { if (/^flash_type=(.*)/) { print lc($$1); }}'; fi)
flashsize = $(shell if test -r $(pdd_txt); then cat $(pdd_txt) | perl -e 'while (<>) { if (/^flash_size=(.*)/) { print $$1; }}'; fi)
pagesize  = $(shell if test -r $(pdd_txt); then cat $(pdd_txt) | perl -e 'while (<>) { if (/^flash_page_size=(.*)/) { print hex $$1; }}'; fi)

default: all

$(bootenv_complete): $(bootenv_txt) $(pdd_txt)
	cat $(bootenv_txt) $(pdd_txt) > $@

$(bootenv_bin): $(bootenv_complete)
	mkbootenv -o $@ $<

$(bootenv_crc): $(bootenv_bin)
	envcrc $< $@

ifeq ($(flash_type), raw_nor)
$(compl_pfi): $(bootenv_crc) $(spl_bin) $(crtm_bin)
else
$(compl_pfi): $(bootenv_complete) $(vmlinux_bin) $(rootfs_bin) $(initramfs_bin) $(spl_bin) $(ipl_bin)
endif
	$(mkpfi) -c $(mkpfi_cfg)
	cp -v $(compl_pfi) $(compl).pfi

# Binary data only
#
$(compl_img): $(compl_pfi)
	$(pfi2bin) -j $(pdd_txt) -o $@ $<



# Default data
#
# If the binary data is not available in the current working directory
# we try to create symlinks to our test data. The parent Makefile may set
# cpu=.fsp0, for example, in the crp_nor subdirectory.

$(vmlinux_bin) $(spl_bin) $(crtm_bin):
	@echo
	@echo "No $@ found, will use default: $(INST_BOOT)/$@$(cpu)"
	@echo "Or press CTRL-C to provide your own" &&	sleep 1 && \
	ln -sf $(INST_BOOT)/$@$(cpu) $@

$(rootfs_bin) $(initramfs_bin):
	@echo
	@echo "No $@ found, will use default: $(IMAGES_ROOT)/$(HOST_ARCH)/$@$(cpu)"
	@echo "Or press CTRL-C to provide your own" &&	sleep 1 && \
	ln -sf $(IMAGES_ROOT)/$(HOST_ARCH)/$@$(cpu) $@

$(ipl_crc32) $(pdd_txt):
	@echo
	@[ ! -r $@ ] && [ ! -h $@ ] &&	\
	echo "No $@ found, will use default: $(DEPTH)/tools/card/data/$(pdd_data)/$@" &&	\
	echo "Or press CTRL-C to provide your own" && sleep 1 &&  \
	ln -sf	$(DEPTH)/tools/card/data/$(pdd_data)/$@ $@

clean:
	$(RM) *.pfi *.mif *.ecc *.img *.oob $(bootenv_complete) \
		$(bootenv_bin) $(bootenv_crc) dummy.bin $(ipl.bin)

_distclean: clean
	$(RM) *.bin $(ipl_crc32) $(pdd_txt) $(pdd_inc)
