#!/usr/bin/env perl

open FILE, $ARGV[0] or die $!;
my  $n_point = $ARGV[1];
my  $real= $ARGV[2];

my @lines = <FILE>;

for (my $i=1 ; $i<$n_point/2+1 ; $i++) {
    # printf $lines[$i];
    chomp $lines[$i];
    my @values= split /\ +/, $lines[$i];

    my $A_r = $values[10];
    my $A_i = $values[11];
    my $B_r = $values[12];
    my $B_i = $values[13];

    if ($real==1) {
        print $A_r."\n";
        print $B_r."\n";
    } else {
        print $A_i."\n";
        print $B_i."\n";

    }
}
