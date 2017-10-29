#!/usr/bin/perl
################################################################################
# $Id: Common.pm,v 1.1 2010/10/14 06:55:26 fldbuild Exp $
#
# Author:       Kirill Kuvaldin (kirill.kuvaldin@auriga.ru)
#
# Organization: Auriga Inc.
#
################################################################################
#
# Contains some common for all testcases functions
#
################################################################################

use strict;
use warnings;
#use lib::FeteServices;
#use lib::TargetServices;

my $testversion="1.1";

sub print_testname
{
  my($testname)=@_;
  print "$testname Ver: $testversion\n";
}

# causes target reboot and performs some initialization commands
#
#
sub reboot
{
  my($target)=@_;
  &init_target_services($target);

  print "Rebooting $target...\n";
  &reboot_target($target);

  # wait approx. 2 minutes (120 seconds)
  my $i;
  my $ok=0;
  for ($i=0;$i<12 && $ok==0;++$i) {
    my $out = `ping -c 1 -w 1 $target`;
    if ($out =~ m/1 packets transmitted, 1 received, 0% packet loss/g) {
      $ok=1;
    }
    if ($ok==0) {
      my $timeleft = ($i+1)*10;
      print "$timeleft seconds left, $target is still not responding\n";
      sleep(10);
    }
  }
  (!$ok) and die "$target hasn't been rebooted correctly.\n";

  # wait for telnet daemon starting
  print "WAITING\n";
  sleep(90);
  &run_init_commands;

}

# invokes some initialization commands
sub run_init_commands
{
  # XXX hardcoded commands

  # &target_command('command'=>"/sbin/ifconfig eth1 192.168.5.20 up");
  &target_command('command'=>
                  "mount -t nfs -o nolock 192.168.1.1:/home/nfs/sunray /mnt/nfs/sunray");
  &target_command('command'=>
                  "test -e /bin/bash || ln -s /mnt/nfs/sunray/bin/bash /bin/bash");
  &target_command('command'=>
                  "test -e /bin/tee || ln -s /mnt/nfs/sunray/bin/tee /bin/tee");
  &target_command('command'=>
                  'echo \'PATH=$PATH:/sbin:/usr/sbin\' >> /etc/profile');
}
