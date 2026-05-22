#include "AssemblyCompiler.h"
#include "Tokenizer.h"
#include "Assembler.h"

//TODO:
//List of everything to do down here:
//Add numbering system support to the assembly language (then port that support over to the AML) such as 0x200 0b1011
//Add the other opcodes and assembly functions to this
//Refactor some of the code (LOWEST PRIORITY)
//Add some more error handling 

std::vector<std::string> assemblyFunctions = {"CLS", "JP", "ADD"};

//This function will check to see if a token is a label that has been created and will get its memory location if so, if not it returns zero
int ifLabelGetMemoryLocation(std::vector<std::string> &labels, std::vector<int> &labelMemoryLocations, std::string &token){
    auto it = std::find(labels.begin(), labels.end(), token); //get our iterator for our finding in the vector

    if(it != labels.end()){ //if the iterator is not the end of the vector then we have found a position
        int index = it - labels.begin(); //calculate the index
        return labelMemoryLocations[index]; //now return the memory location value of the label
    }

    return 0; //return 0 if there is no label location that has been found
}

//This function will take all of the tokens that we have created and then compile them to chip8 opcodes
void compile(std::vector<std::string> &tokens, std::vector<int> &opcodes, std::string outputFile, int &returnCode){
    int romMemoryLocation = 0x200; //this stores our current location in the rom's memory 
    int opcode = 0x0000; //current opcode
    bool opcodeAssembled = 0; //if successfull making opcode
    std::vector<std::string> labels;
    std::vector<int> labelMemoryLocations;

    //This here should satisfy the TODO I have below however I am not entirely sure that I like the system because it just essentially scans the entire file twice
    //TODO: maybe find some other system to do this? I'm not sure on what the best approach would be but maybe I should look into it more, but this actually isn't that expensive in reality
    for(int i = 0; i < tokens.size(); i++){//This for loop block below will literally scan all tokens for label definitions and then create them
        if(tokens[i] == ":"){//if we have a semicolon that means we had a label definition just before hand
            labels.push_back(tokens[i-1]);
            labelMemoryLocations.push_back(romMemoryLocation);
            //no opcode to assemble here
        }

        if(std::find(assemblyFunctions.begin(), assemblyFunctions.end(), tokens[i]) != assemblyFunctions.end())
            romMemoryLocation += 2;
    }

    //now we need to assemble our program correctly into opcodes
    for(int i = 0; i < tokens.size(); i++){
        //TODO: Write a system that will scan for all labels first instead of doing it as the program is compiled
        //The current system has the down side that it will only recognize labels that come before the label is actually put to use by a function
        //The best approach will probably be to just before hand loop through and find all labels and add them first outside of this section
        // if(tokens[i] == ":"){ //if we have a semicolon that means we had a label definition just before hand
        //     labels.push_back(tokens[i-1]);
        //     labelMemoryLocations.push_back(romMemoryLocation);
        //     //no opcode to assemble here
        /*}else*/ 
        //The section above is technically able to be removed but I am leaving that just for now in order to figure out what approach I want to take with solving the issue at hand
        if(tokens[i] == "CLS"){ //clear screen opcode 00E0
            opcode = 0x00E0;
            opcodeAssembled = true; //we have assembled our opcode
        }
        else if(tokens[i] == "JP"){ //jump opcode 1NNN
            opcode = 0x1000;

            //determine if the next token is a memory label
            int labelMemoryLocation = ifLabelGetMemoryLocation(labels, labelMemoryLocations, tokens[i+1]);
            if(labelMemoryLocation > -1) opcode += labelMemoryLocation;

            opcodeAssembled = true;
        }else if(tokens[i] == "ADD"){
            //TODO: make this full assembly opcode work for incrememnting the register by a byte and by a register and incrementing I
            //right now this is just implementing incrementing a register by a byte
            opcode = 0x7000;
            opcode += (std::stoi(tokens[i+1], nullptr, 16) << 8) + (std::stoi(tokens[i+2], nullptr, 16) << 4); //assemble the opcode and for now just assume everything is hexadecimal for now
            opcodeAssembled = true;
        }
        else{ //if we cannot recognize the token based off the rules above then we do not have an opcode to push to our list of assembled opcodes 
            opcodeAssembled = false;
        }

        //if there is an opcode that has been created then we can add it to our list of assembled opcodes
        if(opcodeAssembled)
            opcodes.push_back(opcode);
    }
}

void assembleAssemblyFile(std::vector<char> &ROMBytes, std::string outputFile, int &returnCode){
    std::vector<std::string> tokens; //stores the completed tokens from the tokenization
    std::vector<int> opcodes; //stores the completed opcodes to be assembled

    //first up we need to tokenize the input file, tokenizer code is located in Tokenizer.cpp
    tokenize(ROMBytes, tokens, outputFile, returnCode);
    if (returnCode < 0) return;
    //next up we need to compile the input tokens into opcodes
    compile(tokens, opcodes, outputFile, returnCode);
    if(returnCode < 0) return;
    //then we need to assemble the file properly, assembler code is located in Assembler.cpp
    assemble(opcodes, outputFile, returnCode);
}