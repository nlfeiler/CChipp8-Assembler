# CChipp8-Assembler
A companion assembler program that I wrote for my CHIP-8 emulator called CChipp8. 
This program will assemble CHIP-8 programs that have been written. The programs can either be written straight with the opcodes and assembled or using instruction mnemonic that I have created for the assembler. The program can simply be assembled using any C++ compiler as it only uses standard libraries.  
### CHIP-8 Program Compilation
```
$ ./Assembler -f "ScreenClear.ch8" -tl -o "ScreenClearAssembled.ch8"  
$ ./Assembler -f "ScreenClear.ch8" -op -o "ScreenClearAssembled.ch8"  
```
In the above example compilation the top line is using the built-in assembler mnemonic programming language (the -tl flag is for Typed Language) while the bottom example is assembling a program that has been written directly in CHIP-8 opcodes. (the -op flag is for Opcode)  
The program currently only contains four command line argument flags which are: -f, -tl, -op, -o. A breakdown of the flags is included below.  
```
-f : Denotes the input file for the assembler to look for and utilize.  
-tl : Marks the program to utilize the Typed Language assembly.
-op : Marks the program to utilize the opcode assembly.
-o : Denotes the output file to be written.
```
If the program does not detect an input file flag and an output file flag it will not compile and will return an error. If no language type is marked when the program is ran it will default to opcode assembly.  
### Example Program(s)
```
clearScreen()  
goto(200)
```
The above program is utilizing the assembler's mnemonic language and all that the program will do is clear the screen then jump backwards in the program to clear the screen again infinitely. The assembler currently requires you to keep track of your memory locations in your program however if you need to you can write your program as follows without breaking assembly:
```
0x200: clearScreen()  
0x202: goto(200)  
```
The starting memory location of your program is variable and can be whatever you want it to be, however, you must keep in mind that most CHIP-8 emulators load ROM into memory at 0x200 so it is recommended to start your program at location 0x200 when keeping track.  
  
A full list of the mnemonics for the assembler mnemonic language functions can be found [here](https://github.com/nlfeiler/CChipp8-Assembler/tree/main/Docs/Mnemonic%20Language%20Docs.md) along with some sample programs [here](https://github.com/nlfeiler/CChipp8-Assembler/tree/main/Docs/Example%20Programs)
