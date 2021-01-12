# microc
"microc" is a simple compiler for a C-style language.

# How to compile and run
1. The source code contains one Makefile, type "make" to build an executable microc 
2. type "./microc INPUT_FILE", the compiler will genrate and print out X86 assembly language to the terminal. There are 3 test files provided. 
 - testprog.mc: while loop and if-else conditional statement 
 - testprog2.mc: while loop 
 - testcu.mc: add and minus function calls
3. To save the generated assembly code, redirect the output to a ".asm" file. For example, "./microc testprog.mc > testprog.asm"

