# Shell
This is program for custom shell for commands: ls, mv, rm, cp and cd made in C++.

# COMMANDS TO COMPILE THE PROGRAM:
To compile the program for generating two versions of the binary – one for debugging and another for optimized execution: make all

To compile program for generating binary for debugging: make debug

To compile program for generating for optimization: make release

To compile program to remove all the generated binaries: make clean

To compile program to remove all the generated binaries and generating them again: make clear

# COMMANDS TO RUN THE PROGRAM:
To run debug version: ./result_debug

To run optimized version: ./result_release

To run the program in your system, replace the address in execv() for each of the command to your directory where each object file is present. Eg. execv("~/Shell-cpp/mv.o")

# CONTENTS
Header files: Shell.h cp.h rm.h mv.h ls.h

CPP files: Shell.cpp (contains main() and cd command implementation) cp.cpp rm.cpp mv.cpp ls.cpp

Makefile

profiling_output.txt (output for profiling)

bash_script.sh (bash script for profiling)

ReadMe

# Functionality
ls has options -l, -a, -h, -r, -R
mv has options -f, -v, -i, -h
cp has options -h, -i, -v, -r
cd has options -l, -v, -i, -h
rm has options -h, -i, -v, -r

Commands support wildcards.