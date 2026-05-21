/*
    This serves as a companion program to my Chip8 emulator known as CChipp8 
    This program simply just assembles your chip8 programs into an assembled rom
    This program will allow you to either just write the straight opcodes or it will allow you to use some weird mnemonic programming language that I add
    
    LangMode
        1 - Original CHIP8 Opcode Programming
        2 - Assembler Mnemonic Language
        3 - Assembly language based off Cowgod's CHIP-8 technical reference (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
*/

//TODO: Todo list below:
/*
    Refactor up LanguageCompiler.cpp and OpcodeAssembler.cpp
    RENAME OpcodeAssembler.cpp (I dont think I actually really need to do this because the file is actually just getting assembler here there is no compiling)
    Add some more error handling to the main file here
    Refactor up this main file here as well
    Maybe make some of the comments a little more explanatory for the person that reads them if they are not a dev
    TODO: Port over the advancements of the assembly compiler to the AML compiler (HIGH PRIORITY)
    Comment up the header files and function definitions in source files to make it known what they do
*/

#include "CChipp8Assembler.h"
#include "OpcodeAssembler.h"
#include "LanguageCompiler.h"
#include "AssemblyCompiler.h"

int main(int argc, char** argv){
    std::string fileToAssemble, outputFile; //the fileToAssemble is input path for the input file and outputFile is the output path for the assembled file
    int langMode = 1; //langMode 1 = opcode prorgam, 2 = aml, 3 = assembly
    int returnCode = 1; //return code > 1 means all good, return code < 0 means bad
    bool readyForAssembly = 1; //as long as the file is ready for assembly and nothing goes wrong this should not change values

    //determine the assembler settings based off of the command line args
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-f") fileToAssemble = argv[i+1];
        else if(arg == "-op") langMode = 1; //the file is programmed straight in the opcodes of the chip8
        else if(arg == "-tl") langMode = 2; //the file is programmed in the custom assembler mnemonic code
        else if(arg == "-asm") langMode = 3; //the file is programmed in assembly
        else if(arg == "-o") outputFile = argv[i+1];
    }

    //no command line arg for the file should drop down to here
    //Error code -1
    if (fileToAssemble.empty()) {
        std::cout << "No file has been marked for assembly" << std::endl;
        returnCode = -1;
    }

    std::ifstream file(fileToAssemble, std::ios::binary);
    //no file found should drop down here
    //Error code -2
    if (!file) {
        std::cout << "File not found!" << std::endl;
        returnCode = -2;
    }
    std::cout << "Found file" << std::endl;

    //If an error has occured with how the language mode has been marked then we need to return an error here
    //Error code -5
    if (langMode != 1 && langMode != 2 && langMode != 3) {
        std::cout << "Language mode has not been properly marked" << std::endl;
        returnCode = -5;
    }

    //if we have a negative return code then the file is NOT ready for assembly
    if(returnCode < 0) readyForAssembly = 0;

    //read the file and its bytes then assemble
    try{
        if (readyForAssembly) { //if all of the required things have been taken care of then we can assemble
            //now we need to read the byte and add it to our vector 
            char byte; //stores current byte in input
            std::vector<char> ROMBytes; //this vector stores all the bytes in the input file
            while (file.get(byte)) ROMBytes.push_back(byte); //add the bytes to our vector

            if(langMode == 1) opcodeAssemble(ROMBytes, outputFile, returnCode); //assemble our program into a compiled binary if it is written in just opcodes
            else if(langMode == 2) langCompile(ROMBytes, outputFile, returnCode); //assemble our program into the binary if written with mnemonic language
            else if(langMode == 3) assembleAssemblyFile(ROMBytes, outputFile, returnCode); //assemble our program into the binary if written in assembly
        }
    }catch(std::ios_base::failure error){
        //Error code -6
        std::cout << "An error has occured with reading the file and its bytes." << std::endl;
        returnCode = -6;
    }

    //if there was an error then let it be known
    if(returnCode < 0) std::cout << "An error has occurred at some point during assembly. Please take note of your exit code below and consult the project Github for potential fixes or filing a new issue." << std::endl;

    //give the user their exit code so they can frantically search it on github if something bad happens
    std::cout << "Exit code: " << returnCode << std::endl;
    return returnCode;
}