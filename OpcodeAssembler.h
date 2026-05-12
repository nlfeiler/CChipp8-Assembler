#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>

//the opcode assemble function simply just takes in a vector of the bytes in the rom we need to assemble, a string for the path of the file output and an integer for a return code
//the function just takes the bytes of the ROMBytes vector, turns them into the relevant chip8 opcode with correct formatting and then writes them to the file
//this function is to be used for if the user writes the progam in just the straight opcodes only (ex program: 00E0 1200)
void opcodeAssemble(std::vector<char> &ROMBytes, std::string outputFile, int &returnCode);