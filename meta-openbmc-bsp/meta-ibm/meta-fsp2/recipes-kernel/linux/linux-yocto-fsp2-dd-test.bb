inherit kernel
require recipes-kernel/linux/linux-yocto.inc

# Override SRC_URI in a copy of this recipe to point at a different source
# tree if you do not want to build from Linux' tree.

# Gaurav Linux Image
SRC_URI = "git://github.com/Gauravjsh127/Kernel-source;protocol=git;name=machine"

##### FSP2 Kernel config

SRC_URI += "file://fsp2config.cfg"

LINUX_VERSION ?= "4.10"
LINUX_VERSION_EXTENSION_append = "-custom"

# Modify SRCREV to a different commit hash in a copy of this recipe to
# build a different release of the Linux kernel.
# tag: v4.10-rc7 b917de8a37339e272abe08f4800311a75e4ca35f
SRCREV_machine="b917de8a37339e272abe08f4800311a75e4ca35f"

PV = "${LINUX_VERSION}+git${SRCPV}"

# Override COMPATIBLE_MACHINE to include your machine in a copy of this recipe
# file. Leaving it empty here ensures an early explicit build failure.
COMPATIBLE_MACHINE = "(^$)"


# Avoid QA Issue: No GNU_HASH in the elf binary
INSANE_SKIP_kernel-vmlinux = "ldflags"
