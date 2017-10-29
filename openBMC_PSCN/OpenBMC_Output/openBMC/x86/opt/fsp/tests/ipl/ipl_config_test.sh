#!/bin/sh
for t in *.txt; do
	c=${t/txt/crc}
	sed -e '/^#/d' -e 's/#.*//' $t | \
		../../usr/bin/bootenv -C $c -s 65536 -f -i -
	./config_test $c || {
		echo $t failed
		exit 1
	}
done
