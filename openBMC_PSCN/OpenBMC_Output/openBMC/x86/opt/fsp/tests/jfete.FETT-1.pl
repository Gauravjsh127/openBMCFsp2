#!/usr/bin/perl

#################################################################################
# $Id: jfete.FETT-1.pl,v 1.1 2010/10/14 06:55:26 fldbuild Exp $
#
# Author:        Kirill Gorelov (kirill.gorelov@auriga.ru)
#
# Organization:  Auriga Inc.
#
#################################################################################

use strict;
#use jfete;

my $testname="pingdifsize";
my $description= qq(
################################################################################

jfete.FETT-1     Pinging the target with packets of different sizes

ARGUMENTS:
   target_ip     IP address of the target

DESCRIPTION:

This test case performs pinging the target with packets of different sizes.
It consecutively sends the packets with sizes from 1 to 65507.
If the packet was dropped, it tries to resend the packet.
In case of 5 continuous resendings fail, the test case counts as FAILED.
Otherwise it counts as PASSED.

################################################################################

);

sub print_usage
{
    print "USAGE: jfete.FETT-1.pl target_ip\n";
}

#########
# Main
#########

if ( @ARGV != 1 )
{
    print_usage;
    exit 2;
}

my $host=$ARGV[0];

print $description;

my $maxpacketsize=65507;
my $index;
my $numok=0;
my $numerr=0;
my $resendcnt=0;

for ($index=0; $index<$maxpacketsize; $index++)
{
    if (($index%500)==0)
    {
	print "Current packet size is $index\n";
    }
    my $out = `ping -c 1 -s $index -w 3 $host`;
    if($out =~ m/1 packets transmitted, 1 received, 0% packet loss/g)
    {
	$numok++;
	$resendcnt=0;
    } else
    {
	$numerr++;
	print "Warning! Packet size $index failed!\n";
	if ($resendcnt < 5)
	{
	    print "Resending...\n";
	    $index--;
	    $resendcnt++;
	}
    }
}

# exit code
my $ec=1;

if($numok==$maxpacketsize){$ec=0;}
print "-"x30, " $testname ", "-"x30, "\n";
print "Test Results:\n";
print "		Total transmitted packets: ",$numok+$numerr,"\n";
print "		Successfully transmitted: $numok\n";
print "		Failed transmissions: $numerr\n";
print "		Exit Code: $ec\n";
print "-"x60, "\n";
exit $ec;
