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
    JSR => pack("C", 8),
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


    # Code would look something like
    #
    # try {
    #   for(int i = 0; i < 11; i++) {
    #     print i
    #       if (i > 4) {
    #           throw 42;
    #       }
    #   }
    # } catch(int thrown) {
    #   print "Caught Exception! Value thrown is: #{thrown}\n"
    # };
    # print "End of Script\n";
    my $code = 

        AEX(0, 26, 27, 1). # New Exception for Integer

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

        # Print the variable togehter with some informational string (without modifiying the stack)
        # note the variable itself is print via INSPECT which leaves the variable on the stack
        # The informational string "Counter is: " and the newline are printed with PRINT which removes them
        # from the stack after printing
        SPUSH.
        Str("Counter is: ").
        PRINT.
        INSPECT.
        SPUSH.
        Str("\n").
        PRINT.

        # Add 1 to the variable
        IPUSH. ## Instruction 10
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
        Int(5).

        CMP.
        JL.
        Int(3).

        IPUSH.
        Int(42).
        THROW.


        IPUSH. # Instruction 20
        Int(0).
        LOAD.
        
        IPUSH.
        Int(11).

        # Is our Variable at index 0 less then 10? -> Jump to instruction 3 (instruction index is 0 based)
        CMP.
        JL.
        Int(3).

        JSR.
        Int(33).


        # Exception code
        SPUSH.
        Str("Caught Exception! Value thrown is: ").
        PRINT.
        PRINT. # thrown value
        SPUSH.
        Str("\n").
        PRINT.
        JSR.
        Int(33).

        # end of script
        SPUSH.
        Str("End of Script\n").
        PRINT
        
    ;

    print $fh $code;
    close($fh);
}

sub AEX {
    my ($from, $to, $jump, $type) = @_;
    return pack("C", 20).pack("N!", $from). pack("N!", $to).pack("N!",$jump).pack("N!",$type);
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
