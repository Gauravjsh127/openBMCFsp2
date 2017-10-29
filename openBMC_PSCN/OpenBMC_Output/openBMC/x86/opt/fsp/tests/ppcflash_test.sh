#!/bin/sh -x
# Script to test basic ppcflash/dbgx functionality
# Author: Mike Kobler <mkobler@us.ibm.com>
# Expects to be run from install/x86/tests
# Arguments are the hostname/ports of the riscwatch to use

set -o errexit
set -o nounset

ppcflash="./ppcflash"

if [ -n "${1:-}" ]; then
	ppcflash="$ppcflash --host $1"
else
	echo "Must specify riscwatch hostname"
	exit 1
fi

if [ -n "${2:-}" ]; then
	ppcflash="$ppcflash --port $2"
else
	echo "Using default port of 6470"
fi

dbg="-tv 2"
off=$((992*64*1024))
sz=$((64*1024))
tmpdir=`mktemp -d`
origtmp="$tmpdir/orig"
readtmp="$tmpdir/read"
writetmp="$tmpdir/write"

# save original data
$ppcflash $dbg -R --offset $off --size $sz --file $origtmp

# test changing speeds
$ppcflash $dbg -S 16
$ppcflash $dbg -S 55
$ppcflash $dbg -S 41

# test we can erase flash
$ppcflash $dbg -E --offset $off --size $sz
$ppcflash $dbg -R --offset $off --size $sz --file $readtmp
[ -r $readtmp ] || { echo "No output file found"; exit 1; }
if xxd -p $readtmp | grep -v "f"; then
	echo "Error: block not erased"
	exit 1
fi

# test we can write zeros to flash
dd if=/dev/zero of=$writetmp bs=$sz count=1
$ppcflash $dbg -W --offset $off --size $sz --file $writetmp
$ppcflash $dbg -R --offset $off --size $sz --file $readtmp
[ -r $readtmp ] || { echo "No output file found"; exit 1; }
if xxd -p $readtmp | grep -v "0"; then
	echo "Error: block not written to zeros"
	exit 1
fi

# test we can write random data to flash
dd if=/dev/urandom of=$writetmp bs=$sz count=1
exp=`md5sum $writetmp`
$ppcflash $dbg -W --offset $off --size $sz --file $writetmp
$ppcflash $dbg -R --offset $off --size $sz --file $readtmp
act=`md5sum $readtmp`
if [ "${act/ */}" != "${exp/ */}" ]; then
	echo "Error: md5sum no match"
	exit 1
fi

# restore original data
$ppcflash $dbg -W --offset $off --size $sz --file $origtmp

rm -r $tmpdir
