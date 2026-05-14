# Assembler Mnemonic Language  
The CChipp8-Assembler can utilize one of two language types to compile and assemble your CHIP-8 programs. One of them is by creating your program directly through opcodes or through the Mnemonic Language included within this assembler. The mnemonic language is somewhat different in this assembler is somewhat different from other CHIP-8 interpreters and emulators because it converts to a more C like language with its approach. Most other CHIP-8 interpreters and emulators attempt to convert the instruction set directly into assembly. While the mnemonic language's approach is more C-like there are still some things that you may need to know before beginning to write in the Assembler Mnemonic Language.  
### Getting Started  
First off, like most other programming languages you are not strictly limited to a certain file extension, however I personally like to use .ch8 or .aml for my programs that are targetting this assembler. While you can choose anything, I typically write my programs with a .aml extension and then have the compiled binary output with a .ch8 extension but you can choose whatever extensions you would like. There are a few other things you must know regarding how the CHIP-8 system works before actually beginning to write these programs, or at least its recommended to learn as that will make things easier.  
  
First of all, since this assembler is targetting a system that is being emulated/interpreted the program is getting compiled into a binary and the binary contains program opcodes. Each opcode in CHIP-8 is two bytes length. An example of an opcode would be 00E0, which this opcode simply clears the CHIP-8's display. All opcodes for the CHIP-8 are stored in hexadecimal numbers big-endian style. The opcode given as an example before is a non-variable opcode, meaning that it never changes. That opcode will always do the same thing, however there are opcodes that can vary slightly. For example, there is the opcode commonly referred to as DXYN. This opcode would simply draw to the screen a sprite with a coordinate of X and Y and a height of N. The X in the opcode should represent one of the registers of the system and so should the Y. The N in the opcode should represent a 4 bit constant in hexadecimal. Let's say X is register 0 and Y is register 1 and N is 8, then the opcode when assembled is D018. The way in which these opcodes are assembled should be taken into mind when using the assembler mnemonic language.  
  
The C like syntax of the assembler mnemonic language means that you will be using parentheses after each instruction and most of them will require some sort of parameters. Any instruction that has a variable opcode will require some sort of parameter to be assembled correctly, otherwise you will receive problems with your assembled program. Let's use the opcode DXYN as an example again, this opcode becomes draw in the assembler mnemonic language and will take three parameters. Every part of the opcode that is variable requires being a separate parameter in the assembler mnemonic language. An example of opcode D018 is below:  
```
draw(0, 1, 8)  
```  
In the example above 0 is the X parameter, 1 is the Y parameter and 8 is the N parameter. Writing in the assembler mnemonic language requires that you understand what parameters need to go with each function. Further below a list can be found of all the functions and their corresponding opcode along with an example of what they do.  
  
There are a few more things to keep in mind when writing in the assembler mnemonic language. First and foremost, it is highly recommended to write the memory location of each instruction on the side. This will not break any assembly and will make it easier for you to determine where your jumps are going. It is also highly recommended to start the memory location as 0x200 because most CHIP-8 emulators start reading ROMs into memory at that location. An example is below on how to write the memory location on the side:  
```
0x200: clearScreen()
0x202: goto(200)
```  
Another thing to keep in mind when writing in the assembler mnemonic language is that all values should be written in hexadecimal as the assembler cannot determine the difference between decimal numbers and hexadecimal numbers it assumes that all numbers in hexadecimal numbers. One final thing of note for writing in the assembler mnemonic language is that comments are supported and highly encouraged as even though you are writing in a C like syntax you are still limited to the design structure of an assembly program. Comments in the assembler mnemonic language are started and ended with a pound sign (\#). **ALL comments must be closed otherwise the assembler will continue to read the rest of the program as if it was a comment.**  
  
Example programs can be found [here](https://github.com/nlfeiler/CChipp8-Assembler/tree/main/Example%20Programs/)
### Compiling an Assembler Mnemonic Language Program  
When compiling an assembler mnemonic language program you must pass in the -tl flag to the assembler in order for it to recognize that the program is in the typed assembler mnemonic language instead of being programmed directly in CHIP-8 opcodes. An example compilation command is below:  
```
$ ./Assembler -f "ScreenClear.ch8" -tl -o "ScreenClearAssembled.ch8"  
```
### Assembler Mnemonic Language Functions and their Corresponding Opcodes and Examples  
The format for the list below is as follows: [function and parameters] - [corresponding opcode] - [example usage if applicable]
```
clearScreen() - 00E0
return() - 00EE
goto(NNN) - 1NNN - goto(200)
call(NNN) - 2NNN - call(200)
eqSkip(X, NN) - 3XNN - eqSkip(0, 40)
neqSkip(X, NN) - 4XNN - neqSkip(0, 40)
regEq(X, Y) - 5XY0 - regEq(0, 1)
vxEq(X, Y) - 6XNN - vxEq(0, 0)
vxInc(X, NN) - 7XNN - vxInc(0, 8)
vxSetRegEq(X, Y) - 8XY0 - vxSetRegEq(0, 1)
vxOrVy(X, Y) - 8XY1 - vxOrVy(0, 1)
vxAndVy(X, Y) - 8XY2 - vxAndVy(0, 1)
vxXorVy(X, Y) - 8XY3 - vxXorVy(0, 1)
vxIncVy(X, Y) - 8XY4 - vxIncVy(0, 1)
vxDecVy(X, Y) - 8XY5 - vxDecVy(0, 1)
vxRightShift(X, Y) - 8XY6 - vxRightShift(0, 1)
vxSubVyVx(X, Y) - 8XY7 - vxSubVyVx(0, 1)
vxLeftShift(X, Y) - 8XYE - vxLeftShifg(0, 1)
vxNotEqVy(X, Y) - 9XY0 - vxNotEqVy(0, 1)
setI(NNN) - ANNN - setI(236)
setPC(NNN) - BNNN - setPC(200)
vxRand(X, NN) - CXNN - vxRand(0, 12)
draw(X, Y, N) - DXYN - draw(0, 1, 8)
keyEq(X) - EX9E - keyEq(0)
keyNotEq(X) - EXA1 - keyNotEq(0)
vxEqGetDelay(X) - FX07 - vxEqGetDelay(3)
vxEqGetKey(X) - FX0A - vxEqGetKey(2)
delayEqVx(X) - FX15 - delayEqVx(2)
soundEqVx(X) - FX18 - soundEqVx(3)
iIncVx(X) - FX1E - iIncVx(9)
iEqSprite(X) - FX29 - iEqSprite(5)
dumpVxBCD(X) - FX33 - dumbVxBCD(0)
regDump(X) - FX55 - regDump(9)
redLoad(X) - FX65 - regLoad(9)
```