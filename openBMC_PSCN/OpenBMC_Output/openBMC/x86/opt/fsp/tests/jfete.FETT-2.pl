#!/usr/bin/perl

#################################################################################
# $Id: jfete.FETT-2.pl,v 1.1 2010/10/14 06:55:26 fldbuild Exp $
#
# Author:        Kirill Gorelov (kirill.gorelov@auriga.ru)
#
# Organization:  Auriga Inc.
#
#################################################################################


use strict;
use warnings;

#use jfete;

my $testname="netload";
my $description= qq(
################################################################################

jfete.FETT-2             Flood ping the target

ARGUMENTS:
   target_ip             IP address of the target

DESCRIPTION:

This test case performs flood pinging of the target approximately for 5 minutes.
After that it checks whether the target is still alive. In case it is,
test case counts as PASSED, otherwise -- FAILED.

REQUIREMENTS:

The user must have sudo privileges to run the 'floodping' utility.

################################################################################
);


sub print_usage
{
  print "USAGE: jfete.FETT-2.pl target_ip\n";
}

main();

#################################################################################
#
# main
#
#################################################################################
sub main
{
    my ( $target ) = @ARGV;

    print $description;

    my $pwd = `pwd`;
    chop $pwd;

    my $seconds = 5*60;
    print "running floodping against target (about $seconds seconds)\n";
    my $outp = `sudo $pwd/floodping -t $seconds $target`;

    print "Client output:\ $outp \n";
    if ($outp =~ m/.*packets transmitted.*/)
    {
	#check if the target is still alive
	sleep(3);
	print "Checking if the target is still alive...\n";
	$outp = `ping -c 3 $target`;
	if($outp =~ m/3 packets transmitted, 3 received, 0% packet loss/g)
	{
	    print "-"x30, " $testname ", "-"x30, "\n";
	    print "Test Results:\n";
	    print "		PASSED\n";
	    print "		Exit Code: 0\n";
	    print "-"x60, "\n";
	    exit 0;
	}
	else
	{
	    print "-"x30, " $testname ", "-"x30, "\n";
	    print "Test Results:\n";
	    print "		FAILED: Host $target is not responding !!!\n";
	    print "		Exit Code: 1\n";
	    print "-"x60, "\n";
	    exit 1;
	}
    }
    else
    {
	print "-"x30, " $testname ", "-"x30, "\n";
	print "Test Results:\n";
	print "		FAILED WARNING::Something wrong with floodping\n";
	print "		Exit Code: 1\n";
	print "-"x60, "\n";
	exit 1;
    }
}
