# How to run examples

### 1. Create an executable
you can find a `fvm.pl` file in every example folder. This is a perl script which will generate the bytecode for you
    `perl fvm.pl <filename>`

For example `perl fvm.pl add` will create `add.fvm`

### 2. Run the program
After you created the bytecode file you only need to run it by passing the filepath to the vm
`./flux <filename>`

For example `../../build/bin/flux add.fvm`
