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
Well right now running code is very limited. You can add and subtract Integers do jumps and print strings.

If you want to run example code you should __build the project in release mode__. In debug mode you will get a boatload of noisey debug messages. You can just change the `CMAKE_BUILD_TYPE` from `Debug` to `Release` inside the `build.sh` script
