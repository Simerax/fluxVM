# fluxVM
Might be a vm at some point (its in flux ;) ) for executing bytecode. There is no real goal. I just want to get some experience writing C and I have always been interested in writing vm's, compilers and those kind of things.


# Installation

## Requirements
You will need cmake and a C99 Compiler. There is usage of the `__VA_ARGS__` macro which isn't supported by all compilers. So far this has only been built with GCC 9.3.1 and cmake 3.17.2

## Obtaining the Repository and Buildprocess
Right now there is really only one way to build this project and this will build everything and as debug build

    git clone --recurse-submodules https://github.com/Simerax/fluxVM
    cd fluxVM
    ./build.sh
   
    
    
## Running Code
Well right now running code is very limited. You can do simple Integer addition and store results in variables. Check out the examples directory
