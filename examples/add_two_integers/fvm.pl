#!perl
use warnings;
use strict;



main();
sub main {
    my $filename = $ARGV[0];
    die "You need to supply a filename!" unless defined $filename;
    $filename .= '.fvm' unless $filename =~ /\.fvm$/;
    open(my $fh, '>', $filename) || die "Can't open file '$filename'. Reason: $!";
    binmode($fh);
    my $code = 
        # Push 512 on the stack
        IPUSH(). 
        Int(512).

        # Push 8 on the stack
        IPUSH().
        Int(8).

        # add them together
        IADD().

        # print the result
        IPRINT();
    print $fh $code;
    close($fh);
}


sub IPUSH {
    return pack("C", 3);
}
sub IADD {
    return pack("C", 1);
}
sub IPRINT {
    return pack("C", 7);
}

sub Int {
    return pack("N!", $_[0]);
}
