#!perl
use warnings;
use strict;


use constant {
    IPUSH => pack("C", 3),
    IADD => pack("C",1),
    PRINT => pack("C",7),
    STORE => pack("C",6),
    LOAD => pack("C",5),
    INSPECT => pack("C",15),
    CMP => pack("C",9), 
    JL => pack("C",12), 
    SPUSH => pack("C", 19),
    THROW => pack("C", 21),
};


main();
sub main {
    my $filename = $ARGV[0];
    die "You need to supply a filename!" unless defined $filename;
    $filename .= '.fvm' unless $filename =~ /\.fvm$/;
    open(my $fh, '>', $filename) || die "Can't open file '$filename'. Reason: $!";
    binmode($fh);
    my $code = 

        IPUSH.
        Int(5).

        THROW
    ;

    print $fh $code;
    close($fh);
}


sub Str {
    my ($str) = @_;
    unless ($str =~ /\0$/) {
        $str .= "\0";
    }
    my $len;
    { use bytes; $len = length($str); }
    return Int($len) . $str;
}

sub Int {
    return pack("N!", $_[0]);
}
