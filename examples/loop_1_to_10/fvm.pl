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
};


main();
sub main {
    my $filename = $ARGV[0];
    die "You need to supply a filename!" unless defined $filename;
    $filename .= '.fvm' unless $filename =~ /\.fvm$/;
    open(my $fh, '>', $filename) || die "Can't open file '$filename'. Reason: $!";
    binmode($fh);
    my $code = 
        # Create new variable at index 0 with value 1 and store it
        IPUSH.
        Int(1).
        IPUSH.
        Int(0).
        STORE.


        # Put the variable with index 0 on the stack
        IPUSH. # <- this is instruction 3
        Int(0).
        LOAD.

        # Print the variable (without modifiying the stack)
        INSPECT.

        # Add 1 to the variable
        IPUSH.
        Int(1).
        IADD.

        # Store the Result back in Variable at index 0 so that we can reuse it after the jump
        IPUSH.
        Int(0).
        STORE.

        # load the variable from index 0 to compare it against 10 (10 is end of loop)
        IPUSH.
        Int(0).
        LOAD.

        IPUSH.
        Int(11).

        # Is our Variable at index 0 less then 10? -> Jump to instruction 3 (instruction index is 0 based)
        CMP.
        JL.
        Int(3)
    ;

    print $fh $code;
    close($fh);
}



sub Int {
    return pack("N!", $_[0]);
}
