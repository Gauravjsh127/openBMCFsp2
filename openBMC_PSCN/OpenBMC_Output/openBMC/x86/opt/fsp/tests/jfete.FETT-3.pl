#!/usr/bin/perl

#################################################################################
# $Id: jfete.FETT-3.pl,v 1.1 2010/10/14 06:55:26 fldbuild Exp $
#
# Author:        Kirill Gorelov (kirill.gorelov@auriga.ru)
#
# Organization:  Auriga Inc.
#
#################################################################################

use strict;
use warnings;

#use jfete;

my $testname="netload2";
my $description= qq(
################################################################################

jfete.FETT-3         Flood ping the target

ARGUMENTS:
   target_ip         IP address of the first interface
   target_ip2        IP address of the second interface

DESCRIPTION:

This test case performs flood pinging of the both interfaces of the target
approximately for 5 minutes. After that it checks whether both interfaces of the
target are still alive. In case they are, test case counts as PASSED,
otherwise -- FAILED.

REQUIREMENTS:

The user must have privileges to run 'floodping' utility.

################################################################################
);


sub print_usage
{
    print "USAGE: jfete.FETT-3.pl target_ip target_ip2\n";
}

main();

#################################################################################
#
# main
#
#################################################################################
sub main 
{
    my ( $ip1, $ip2 ) = @ARGV;

    print $description;

    my $pwd = `pwd`;
    chop $pwd;

    my $seconds = 5*60;

    print "running floodping against both target ip adresses (about $seconds seconds)\n";
    my $outp = `sudo $pwd/floodping -t $seconds $ip1 & sudo $pwd/floodping -t $seconds $ip2`;

    sleep(10);

    $outp =~ s/\n/ /g;
    print "Client output:\ $outp \n";
    if ($outp =~ m/.*packets transmitted.*packets transmitted.*/)
    {
	#check if the target is still alive
	print "Checking if the target is still alive...\n";
	my $outp = `ping -c 3 $ip1`;
	my $outp2 = `ping -c 3 $ip2`;
	
	if(($outp =~ m/3 packets transmitted, 3 received, 0% packet loss/g) &&
	   ($outp2 =~ m/3 packets transmitted, 3 received, 0% packet loss/g))
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
	    print "		FAILED: ip $ip1 and/or $ip2 is not responding !!!\n";
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
