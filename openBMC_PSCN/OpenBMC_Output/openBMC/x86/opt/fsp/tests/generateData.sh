#!/bin/bash

# Script to generate html code coverage files from data collected after a fete run.
# Author Aravind T Nair <aravindnair@in.ibm.com>


#get the base directory of the Git repository
BASE_DIR=`git rev-parse --show-toplevel`

if [ -z "$BASE_DIR" ]; then
	echo "ERR: Invalid Git repository?"
	RC=-1
	exit $RC
fi

echo
echo "FAKEROOT=$FAKEROOT"
source $FAKEROOT/opt/exportroot/etc/mcp.manifest
echo mcp_arch=${_MCP_ARCH}
echo

KSRC=$BASE_DIR/kernel/linux.${_MCP_ARCH}
LCOV="${_MCP_ARCH}-mcp81-jail lcov"
GENHTML="${_MCP_ARCH}-mcp81-jail genhtml"
GCOV_PATH=tests/gcov/data

cd $BASE_DIR/$GCOV_PATH

let TEST_NUM=0

# Create driver and/or apps .info file for each coverage data tarball
GEN_INFO()
{
	DATA=${1-data.tgz}
	let TEST_NUM=$TEST_NUM+1

	#extract the driver coverage data
	DATA_TARBALL=$BASE_DIR/$GCOV_PATH/$DATA
	if [ ! -e $DATA_TARBALL ]; then
		echo "ERR: File $DATA_TARBALL does not exist, gcov not enabled?"
		RC=0 #to prevent fete from failing if gcov isn't enabled
		exit $RC
	fi
	
	echo "Cleaning pre-existing .gcda files from Git repository"
	find $BASE_DIR/drivers $BASE_DIR/apps -name '*.gcda' -print | xargs -n10 rm -vf
	RC=$?
	if [ $RC -ne 0 ]; then
	        echo "ERR: Cleaning old .gcda from Git repository failed"
	        exit $RC
	fi

	echo "Extracting $DATA_TARBALL..."
	tar xzf $DATA_TARBALL
	RC=$?
	if [ $RC -ne 0 ]; then
	        echo "ERR: Extracting $DATA_TARBALL failed"
	        exit $RC
	fi
	
	#copy the .gcda files to the right paths in the Git repository
	echo "Copying .gcda files to the correct paths..."
	rsync -avzK sys/kernel/debug/gcov/$BASE_DIR/drivers/ $BASE_DIR/drivers
	rsync -avz ${BASE_DIR:1}/ $BASE_DIR
	
	#cleanup the mess
	rm -rf sys
	rm -rf home
	
	# run lcov and generate the .info file
	echo "Running lcov to generate the html_drivers/driver.${TEST_NUM}.info file..."
	$LCOV -b $BASE_DIR -k $KSRC --directory $BASE_DIR/drivers --capture \
			--output-file html/html_drivers/driver.${TEST_NUM}.info
	# (dont need -k )
	RC=$?
	if [ $RC -ne 0 ]; then
	        echo "ERR: Running lcov failed on driver data"
	fi
	
	#cleanup the .info file of unwanted files
	echo "Running lcov to cleanup the generated html_drivers/driver.${TEST_NUM}.info file..."
	$LCOV -q --remove html/html_drivers/driver.${TEST_NUM}.info "include/linux*" "include/trace/*" \
			"include/asm-generic*" "pre_processor/trexhash.c" "kernel/linux.${_MCP_ARCH}/arch*"\
			-o html/html_drivers/driver.${TEST_NUM}.info
	RC=$?
	if [ $RC -ne 0 ]; then
	        echo "ERR: lcov error, pruning driver.info"
	fi

	#allow cases where we are missing either apps or tools data
	unset APPS_GCDA TOOLS_GCDA
	test "`find $BASE_DIR/apps -name '*.gcda' | wc -l`" != "0" && APPS_GCDA="--directory $BASE_DIR/apps"
	test "`find $BASE_DIR/tools -name '*.gcda' | wc -l`" != "0" && TOOLS_GCDA="--directory $BASE_DIR/tools"
	
	if [ "$APPS_GCDA||$TOOLS_GCDA" != "||" ]; then
		# running lcov to create the app.info file
		echo "Running lcov to generate the html_apps/app.${TEST_NUM}.info file..."
		$LCOV $APPS_GCDA $TOOLS_GCDA --capture --output-file html/html_apps/app.${TEST_NUM}.info
		RC=$?
		if [ $RC -ne 0 ]; then
		        echo "ERR: Running lcov failed on app data"
		fi
	fi
	
	$LCOV --remove html/html_apps/app.${TEST_NUM}.info "/opt/mcp/${_MCP_ARCH}*" "tools/flashutils/lib*" \
			--output-file html/html_apps/app.${TEST_NUM}.info
	RC=$?
	if [ $RC -ne 0 ]; then
	        echo "ERR: lcov error, pruning app.info"
	fi
}

# prepare to generate html data
rm -rf html
mkdir -p html/html_drivers
mkdir -p html/html_apps

#Generate a test coverage .info file for file argument (or just use data.tgz default)
Test_List=`eval echo ${*-data.tgz}`
echo "Process Test Data files: $Test_List"
for T in $Test_List
do
	echo "Creating .info file for $T"
	GEN_INFO $T
done

#Combine coverage data from all .info files generated

unset PList
for D in html/html_drivers/driver.*.info
do
	PList="$PList -a $D"
done

echo "Running lcov to merge html_drivers/driver.info"
echo "PList=$PList"
$LCOV -b $BASE_DIR -k $KSRC --directory $BASE_DIR/drivers $PList \
			--output-file html/html_drivers/driver.info

unset PList
for D in html/html_apps/app.*.info
do
	PList="$PList -a $D"
done

if [ "$PList" != "" ]; then
	$LCOV $APPS_GCDA $TOOLS_GCDA $PList --output-file html/html_apps/app.info
	RC=$?
	if [ $RC -ne 0 ]; then
		echo "ERR: Running lcov failed on app data"
	fi
	$LCOV --remove html/html_apps/app.info "/opt/mcp/${_MCP_ARCH}*" "tools/flashutils/lib*" \
			--output-file html/html_apps/app.info
	RC=$?
	if [ $RC -ne 0 ]; then
		echo "ERR: lcov error, pruning app.info"
	fi
fi

#finally generate the html files
echo "Generating html files..."
$GENHTML html/html_drivers/driver.info -p $BASE_DIR -o html/html_drivers
if [ -f html/html_apps/app.info ]; then
	$GENHTML html/html_apps/app.info -p $BASE_DIR -o html/html_apps
else
	echo "No Application Coverage Data found" >html/html_apps/index.html
fi

#index.html should give the coverage data for the BB custom drivers & apps
if [ ! -e html/html_drivers/index.html ]; then
   echo "ERR: Missing html index file"
fi

if [ ! -e html/html_apps/index.html ]; then
   echo "ERR: Missing html index file"
fi

# everything completed succesfully
echo "Driver coverage data here: $BASE_DIR/$GCOV_PATH/html/html_drivers/index.html"
echo "Apps coverage data here: $BASE_DIR/$GCOV_PATH/html/html_apps/index.html"
RC=0

# create an index.html file on top to point the above generated files..
create_index_html()
{
	echo -e "<!DOCTYPE html>\n" > html/index.html

	echo "<html>" >> html/index.html
	echo "<head>" >> html/index.html
	echo "<title>Building Block LCov/GCov Coverage</title>" >> html/index.html
	echo -e "</head>\n" >> html/index.html

	echo "<body>" >> html/index.html
	echo -e "<h1>Building Block LCov/GCov Code Coverage Measurement</h1>\n" >> html/index.html

	echo -e "<p>The latest GCov run on HEAD. This is from jfete run on FSP-2 simics.<br>\n" >> html/index.html

	echo "<a href="html_drivers/index.html">Driver coverage data</a>" >> html/index.html
	echo "<br>" >> html/index.html
	echo "<a href="html_apps/index.html">Apps/Tools coverage data</a>" >> html/index.html
	echo -e "<hr>\n" >> html/index.html

	echo -e "</body>\n" >> html/index.html

	echo "</html>" >> html/index.html
}

create_index_html
