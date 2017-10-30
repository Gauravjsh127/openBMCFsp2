inherit kernel
require recipes-kernel/linux/linux-yocto.inc

# Override SRC_URI in a copy of this recipe to point at a different source
# tree if you do not want to build from Linus' tree.

SRC_URI = "git://github.com/fr0st61te/linux;protocol=git;name=machine"

##### FSP2 Kernel config file

SRC_URI += "file://fsp2_defconfig.cfg"

#### FSP Header files added
SRC_URI += "file://1_headerfiles.patch"

#### FSP fsptrace device driver patch added
SRC_URI += "file://2_fsptrace.patch"

#### FSP NGFSI device driver patch added
SRC_URI += "file://3_ngfsi.patch"

#### FSP NGFSI device driver patch added
SRC_URI += "file://4_pra.patch"

#### FSP Device tree patch
SRC_URI += "file://5_device_tree.patch"

#### FSP pabend patch
SRC_URI += "file://6_pabend.patch"

#### FSP Ethernet device driver patch
SRC_URI += "file://7_ibm-emac.patch"

LINUX_VERSION ?= "4.10"

LINUX_VERSION_EXTENSION_append = "-custom"

# Modify SRCREV to a different commit hash in a copy of this recipe to
# build a different release of the Linux kernel.
# tag: v4.10-rc7 ba5c00e0627e4d735b36e0353744c6c3c4690454
###   https://github.com/fr0st61te/linux

SRCREV_machine="e4eaca0332d5178a8dcb1d30f3f5b6c370a7132a"

PV = "${LINUX_VERSION}+git${SRCPV}"

# Override COMPATIBLE_MACHINE to include your machine in a copy of this recipe
# file. Leaving it empty here ensures an early explicit build failure.
COMPATIBLE_MACHINE = "(^$)"

# Avoid QA Issue: No GNU_HASH in the elf binary
INSANE_SKIP_kernel-vmlinux = "ldflags"


