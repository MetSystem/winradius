#!/usr/bin/perl
use strict;
use warnings;
use IO::File;


my $dir_to_proc="F:\\svn\\existing-winradius-code-0\\winradius-code-0\\WinRadius\\rlm_eap";
my $file;
opendir DH ,$dir_to_proc;
foreach (readdir DH) {
	next unless $_ =~ /\.cpp$/;
	s/\.cpp/\.obj/;
	printf( "$_ ");
}
printf("\n");