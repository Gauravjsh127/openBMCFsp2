#!/usr/bin/perl

#################################################################################
# $Id: jfete.FETT-10.pl,v 1.1 2010/10/14 06:55:26 fldbuild Exp $
#
# Author:        Kirill Gorelov (kirill.gorelov@auriga.ru)
#
# Organization:  Auriga Inc.
#
#################################################################################

use strict;
use warnings;

use jfete;


sub print_usage
{
    print "USAGE: jfete.FETT-10.pl target_ip\n";
}

my $testname="netperftest";
my $description= qq(
#################################################################################

jfete.FETT-10    Tests the data transfer rate using NetPerf

ARGUMENTS:
   target_ip     IP address of the target

DESCRIPTION:

This tests performs testing the data transfer rate using NetPerf
utility. It measures the throughput of the target. If it exceeds 50Mbits/sec
the test case PASSED, otherwise - FAILED.

#################################################################################

);

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

    my $cctestdir = "/mnt/opt/fsp/tests";
    my $ec;

    # try to kill all running netserver applications on the target
    jfete_telnetcmd( $target, "killall netserver" );

    # check if netserver is executable on the target
    $ec = jfete_telnetcmd( $target, "test -x $cctestdir/edd/target/netserver" );
    die( "$cctestdir/edd/target/netserver not found on target $target\n" ) if( $ec != 0 );

    # start netserver on target
    print "starting netserver on target\n";
    jfete_telnetcmd( $target, "$cctestdir/edd/target/netserver &" );

    # run netperf
    print "starting netperf on host\n";
    my $outp = `./netperf -H $target`;
    print "Client output:\ $outp \n";
    $outp =~ s/\n/ /g;
    my $bandwidth=0;
    if ($outp =~ m/.*(\d+)\s+(\d+)\s+(\d+)\s+([+-]?\d+[\.,]?\d*)\s+([+-]?\d+[\.,]?\d*).*/g)
    {
	$bandwidth=$5;
	print "Throughput Mbits/sec is $bandwidth\n";
	if ($bandwidth>50)
	{
	    print "Test PASSES\n";
	    $ec=0;
	} else {
	    print "Test FAILED\n";
	    $ec=1;
	}
    } else {
	print "Output is not recognized\nTest FAILED\n";
	$ec=1;
    }

    # try to kill all running netserver applications on the target
    jfete_telnetcmd( $target, "killall netserver" );

    print "-"x30, " $testname ", "-"x30, "\n";
    print "Test Results:\n";
    print "              Bandwidth: $bandwidth Mbits/sec\n";
    print "              Exit Code: $ec\n";
    print "-"x60, "\n";
    exit $ec;
}
