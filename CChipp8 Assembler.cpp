/*
    This serves as a companion program to my Chip8 emulator known as CChipp8 
    This program simply just assembles your chip8 programs into an assembled rom
    This program will allow you to either just write the straight opcodes or it will allow you to use some weird mnemonic programming language that I add
    
    LangMode
        1 - Original CHIP8 Opcode Programming
        2 - Assembler Mnemonic Language
*/

#include "CChipp8 Assembler.h"
#include "OpcodeAssembler.h"
#include "LanguageCompiler.h"

int main(int argc, char** argv){
    std::string fileToAssemble, outputFile;
    int langMode = 1;
    int returnCode = 1;
    bool readyForAssembly = 1;

    //determine the assembler settings based off of the command line args
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-f") fileToAssemble = argv[i+1];
        else if(arg == "-op") langMode = 1; //the file is programmed straight in the opcodes of the chip8
        else if(arg == "-tl") langMode = 2; //the file is programmed in the custom assembler mnemonic code
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
    if (langMode != 1 && langMode != 2) {
        std::cout << "Language mode has not been properly marked" << std::endl;
        returnCode = -5;
    }

    //if we have a negative return code then the file is NOT ready for assembly
    if(returnCode < 0) readyForAssembly = 0;

    if (readyForAssembly) { //if all of the required things have been taken care of then we can assemble
        //now we need to read the byte and add it to our vector 
        char byte;
        std::vector<char> ROMBytes;
        while (file.get(byte)) {
            if(byte != ' ' && byte != '\n' && byte != '\r') //dont add the byte if it is a space or newline
                ROMBytes.push_back(byte); //add the bytes to our vector
        }

        if(langMode == 1) opcodeAssemble(ROMBytes, outputFile, returnCode); //assemble our program into a compiled binary if it is written in just opcodes
        else if(langMode == 2) langCompile(ROMBytes, outputFile, returnCode); //assemble our program into the binary if written with mnemonic language
    }

    //if there was an error then let it be known
    if(returnCode < 0) std::cout << "An error has occurred at some point during assembly. Please take note of your exit code below and consult the project Github for potential fixes or filing a new issue." << std::endl;

    std::cout << "Exit code: " << returnCode << std::endl;
    return returnCode;
}