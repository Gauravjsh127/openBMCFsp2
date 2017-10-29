# !/bin/bash
#
# Author: Randal Allen Anderson III
#
# This test-script generates a directory with some ordinary text-files
# a subdirectory, a binary file and some links (soft and hard) to see
# wheter zofs/mkzofs is capable of handling this correctly.
#
# Modified by Neelesh Gupta
#
# This tests the new capability of mkzofs to read the order of the desired
# data file from a file listing the files to be written in order.

MKZOFS=mkzofs
MK_DIR_NAME=order_dir

if [ -d $MK_DIR_NAME ]; then
  rm -rf $MK_DIR_NAME
fi

mkdir $MK_DIR_NAME
mkdir $MK_DIR_NAME/test_l2

echo key1 > $MK_DIR_NAME/test_l2/key1
echo key2 > $MK_DIR_NAME/key2
dd if=/dev/zero of=$MK_DIR_NAME/data1.bin bs=1024 count=10
echo key3 > $MK_DIR_NAME/key3
echo duplicate > $MK_DIR_NAME/duplicate

pushd .
cd $MK_DIR_NAME/test_l2
ln  -sf ../key2 key2
popd

dd if=/dev/zero of=$MK_DIR_NAME/data2.bin bs=1 count=1025
dd if=/dev/zero of=$MK_DIR_NAME/test_l2/onebyte bs=1 count=1

echo key4 > $MK_DIR_NAME/key4
echo key4_1 > $MK_DIR_NAME/key4_1

ls -lR $MK_DIR_NAME
echo -e "\n"

./$MKZOFS -m zofs.mdata -d zofs.data -D $MK_DIR_NAME -O order.cfg
RC=$?
if [ $RC -ne 0 ]; then
	echo Error, running mkzofs using ordered list file.
	exit $RC
fi

./$MKZOFS -m zofs1.mdata -d zofs1.data -D $MK_DIR_NAME -O order1.cfg
RC=$?
if [ $RC -ne 0 ]; then
        echo -e "Expected error as the file listed in the ordered list file\
\n'order1.cfg' can't be located within directory tree.\n"
	RC=0
fi

./$MKZOFS -m zofs2.mdata -d zofs2.data -D $MK_DIR_NAME -O order2.cfg
RC=$?
if [ $RC -ne 0 ]; then
        echo -e "Expected error as the file listed in the ordered list file\
\n'order2.cfg' is repeated (duplicate).\n"
        RC=0
fi

if [ -d $MK_DIR_NAME ]; then
	rm -rf $MK_DIR_NAME
	rm zofs*data
fi

exit $RC
