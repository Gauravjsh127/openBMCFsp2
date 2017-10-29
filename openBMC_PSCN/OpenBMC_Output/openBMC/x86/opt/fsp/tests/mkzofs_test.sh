# !/bin/bash
#
# mkzofs_test.sh
#
# Author: Neelesh Gupta
#
# This test script create a multi-level directory tree structure having
# directories, files and softlinks. The 'mkzofs' tool work over the created
# directory and generates the '.mdata' & '.data' files useful for ZOFS.
# To verify these meta-data and data files, the 'unmkzofs' tool uses them to
# build the directory tree. These two directory tree structures should match..
#
# The other one (mkzofs_orderlist.sh) tests the new capability of mkzofs to read
# the order of the desired data file from a file listing the files to be written
# in order.

MKZOFS=mkzofs
UNMKZOFS=unmkzofs

if [ ${PWD##*/} == "mkzofs" ]
then
	# running from the source path (bb/tools/mkzofs)
	echo $PWD
	DEPTH=../..
else
	# running from the jfete installed path (bb/install/x86/tests)
	echo $PWD
	DEPTH=../../..
fi

export LD_LIBRARY_PATH=$DEPTH/install/x86/usr/lib/

MK_DIR_NAME=mk_dir
UNMK_DIR_NAME=unmk_dir

if [ -d $MK_DIR_NAME ]; then
	rm -rf $MK_DIR_NAME
fi

if [ -d $UNMK_DIR_NAME ]; then
        rm -rf $UNMK_DIR_NAME
fi

mkdir $MK_DIR_NAME
mkdir $UNMK_DIR_NAME

# Creates the directory structure
create_dir_tree()
{
	# create the directory tree, let's say level1 5 subdirs, level2 4 subdirs
	# level3 3 subdirs...
	mkdir -p $MK_DIR_NAME/{dir0,dir1,dir2,dir3,dir4}/{dir0,dir1,dir2,dir3}/{dir0,dir1,dir2}

	# create the new files, let's say 4 files within each subdir
	find $MK_DIR_NAME -type d -exec touch {}/file{0,1,2,3} \;

	# write some data to all these files created above
	find ./$MK_DIR_NAME -type f -exec sh -c 'echo "registry_data" > {}' \;

	# create the softlinks
	for dir in `find $MK_DIR_NAME -type d`
	do
		cd $dir
		find -maxdepth 1 -type f -exec ln -s {} "{}.$RANDOM" \;
		cd - > /dev/null
	done
}

# Runs the 'mkzofs' and 'unmkzofs' executables and compare the dirs
test_mkzofs()
{
	echo ./$MKZOFS -D $MK_DIR_NAME
	./$MKZOFS -D $MK_DIR_NAME
	RC=$?
	if [ $RC -ne 0 ]; then
		echo Error, running mkzofs
		exit $RC
	fi

	echo ./$UNMKZOFS -i $UNMK_DIR_NAME
	./$UNMKZOFS -i $UNMK_DIR_NAME
	RC=$?
	if [ $RC -ne 0 ]; then
                echo Error, running unmkzofs
                exit $RC
        fi

	echo Compare the mk_dir and unmk_dir
	diff -r $MK_DIR_NAME $UNMK_DIR_NAME
	RC=$?
	if [ $RC -ne 0 ]; then
                echo FAIL. Basic test for 'mkzofs' and 'unmkzofs'.
                exit $RC
        fi

	echo -e "\nPASS. Basic test for 'mkzofs' and 'unmkzofs'.\n"
}

echo Create the directory tree structure
create_dir_tree
echo Run mkzofs and ummkzofs executables
test_mkzofs

# remove the above created dirs 'mk_dir' & 'unmk_dir'
if [ -d $MK_DIR_NAME ]; then
	rm -rf $MK_DIR_NAME
	rm zofs*data
fi

if [ -d $UNMK_DIR_NAME ]; then
        rm -rf $UNMK_DIR_NAME
fi

# 'mkzofs' test with ordered list as input
echo -e "Order list test.."
echo -e "Take ordered list as input to 'mkzofs' and check the expected output"

./mkzofs_orderlist.sh
RC=$?

if [ $RC -ne 0 ]; then
	echo -e "FAIL. Order list test.\n"
	exit $RC
fi

echo -e "PASS. Order list test.\n"

exit $RC
