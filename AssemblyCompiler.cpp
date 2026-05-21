#include "AssemblyCompiler.h"

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

//This function will write all of the opcodes in the opcodes vector to the output file and then set the return code appropriately
void assemble(std::vector<int> &opcodes, std::string outputFile, int &returnCode){
    try {
		std::ofstream fileOutput(outputFile, std::ios::binary);
		for (int i = 0; i < opcodes.size(); i++) {
			std::cout << "Opcode: " << std::hex << opcodes[i] << std::endl; //print out each opcode that we are writing

			char highByte = (opcodes[i] >> 8) & 0xFF; //now we need to get the high and low byte of the opcode
			char lowByte = opcodes[i] & 0xFF; //get the low byte

			fileOutput.write(&highByte, sizeof(highByte)); //write the high byte
			fileOutput.write(&lowByte, sizeof(lowByte)); //write the low byte
		}
	}
	catch (std::ios_base::failure& e) {//if some error has happened we will tell the user and exit
		std::cout << "Error assembling file. Please try again.\n" << e.what() << std::endl;
		returnCode = -4; //we are going to return error code -4 here for this error
		return;
	}

	std::cout << "Successfully assembled file" << std::endl;
	returnCode = 1; //set return code to 1 to signifiy successful output
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

void tokenize(std::vector<char> &ROMBytes, std::vector<std::string> &tokens, std::string outputFile, int &returnCode){
    std::string token = ""; //store the current working token
    bool inComment = 0; //store whether or not we are currently within a comment in our code

    //iterate through all of the bytes of the original file
    for(int i = 0; i < ROMBytes.size(); i++){
        if(ROMBytes[i] == ';'){ //if the current char is a semi colon we need to eat it and then indicate we have started a comment
            inComment = true;
            continue; //move on to the next byte
        }

        //if current char is a space then we can flush the current token and eat the space
        if(ROMBytes[i] == ' '){
			if(!token.empty()){//flush char
				tokens.push_back(token);
				token = "";
			}

			continue; //carry on to next byte in original file
		}

        //if a new line is presented we also need to eat that line and flush the current token as well as mark we are no longer in a comment
        if(ROMBytes[i] == '\n' || ROMBytes[i] == '\r' || ROMBytes[i] == '\0'){
            inComment = false; //no longer in comment if a new line has happened

            if(!token.empty()){ //flush the current token
                tokens.push_back(token);
                token = "";
            }

            continue; //carry on
        }

        //if we are not on a special char and not in a comment then add the current byte to our working token 
        if((ROMBytes[i] != ':' && ROMBytes[i] != ' ' && ROMBytes[i] != ',' && ROMBytes[i] != '\n') && !inComment)
            token += ROMBytes[i];
        else{ //if we are on a special char we need to flush token and then push the special char to the tokens
            if(token != "")
                tokens.push_back(token);
            tokens.push_back(std::string(1, ROMBytes[i]));
            token = "";
        }
    }

    //push our final token after the loop provided that it is not an empty token
    if(token != "") tokens.push_back(token);
}

void assembleAssemblyFile(std::vector<char> &ROMBytes, std::string outputFile, int &returnCode){
    std::vector<std::string> tokens; //stores the completed tokens from the tokenization
    std::vector<int> opcodes; //stores the completed opcodes to be assembled

    //first up we need to tokenize the input file
    tokenize(ROMBytes, tokens, outputFile, returnCode);
    if (returnCode < 0) return;
    //next up we need to compile the input tokens into opcodes
    compile(tokens, opcodes, outputFile, returnCode);
    if(returnCode < 0) return;
    //then we need to assemble the file properly
    assemble(opcodes, outputFile, returnCode);
}