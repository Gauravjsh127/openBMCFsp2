#!/usr/bin/perl

#################################################################################
# $Id: jfete.FETT-8.pl,v 1.1 2010/10/14 06:55:26 fldbuild Exp $
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
    print "USAGE: jfete.FETT-8.pl target_ip\n";
}

my $testname="diffmtu";
my $description= qq(
#################################################################################

jfete.FETT-8     Pinging the target with different MTUs

ARGUMENTS:
   target_ip     IP address of the target

DESCRIPTION:

This test case pings the target with the different MTUs.
To change MTU on the target it invokes the command 'ifconfig eth mtu index'
where variable index is changed from 100 to 1500 with step of 100 units.
If the packet was dropped, it tries to resend the packet.
In case of 5 continuous resendings fail, the test case counts as FAILED.
Otherwise it counts as PASSED.

#################################################################################

);

#########
# Main
#########

if ( @ARGV != 1 )
{
    print_usage;
    exit 2;
}

my $target=$ARGV[0];

print $description;

my $step=100;
my $maxpacketsize=1500;
my $index=0;
my $numok=0;
my $numerr=0;
my $resendcnt=0;
my $startmtu=100;
my $maxmtu=1500;
my $numpings=($maxmtu-$startmtu+$step);

# Perform data tranfer with different MTUs
for ($index=$startmtu; $index<=$maxmtu; $index+=$step)
{
    my $cmd = "ifconfig eth0 mtu $index";
    my $ec = jfete_telnetcmd( $target, $cmd );
    die( "failed to run command on cc: $cmd\n" ) if( $ec != 0 );

    sleep(2);
    
    my $outp = `ping -c 1 -s $maxpacketsize $target`;
    if ($outp =~ m/1 packets transmitted, 1 received, 0% packet loss/g) {
	$numok++;
	$resendcnt=0;
    } else {
	$numerr++;
	print "Warning! Packet with MTU $index failed!\n";
	if ($resendcnt < 5) {
	    print "Resending...\n";
	    $index--;
	    $resendcnt++;
	}
    }
}

my $rc = 1;

if($numok==($numpings/$step)){$rc=0;}
print "-"x30, " $testname ", "-"x30, "\n";
print "Test Results:\n";
print "		Total transmitted packets: ",$numok+$numerr,"\n";
print "		Successfully transmitted: $numok\n";
print "		Failed transmissions: $numerr\n";
print "		Exit Code: $rc\n";
print "-"x60, "\n";

exit $rc;
