#include "LanguageCompiler.h"

//this array actually serves no purpose but is here just in case for easy access to mnemonics in the code without consulting other documentation
std::string langCodes[35] = {
	"clear_screen", "return", "goto", "call", "eqSkip", "neqSkip", "regEq", "vxEq", "vxInc", "vxSetRegEq", "vxOrVy", "vxAndVy",
	"vxXorVy", "vxIncVy", "vxDecVy", "vxRightShift", "vxSubVyVx", "vxLeftShift", "vxNotEqVy", "setI", "setPC", "vxRand", "draw",
	"keyEq", "keyNotEq", "vxEqGetDelay", "vxEqGetKey", "delayEqVx", "soundEqVx", "iIncVx", "iEqSprite", "dumpVxBCD", "regDump", "regLoad"
};

//get token N which should be the last token in the parentheses of all opcodes that use it
int getN(std::vector<std::string>& tokens, int index) {
	return std::stoi(tokens[index+6], nullptr, 16);
}

//get NN which should be the second token in the parentheses of all opcodes
int getNN(std::vector<std::string> &tokens, int index) {
	return std::stoi(tokens[index+4], nullptr, 16);
}

//get Y which should also be the second token in all parentheses if used
int getY(std::vector<std::string> &tokens, int index) {
	return std::stoi(tokens[index+4], nullptr, 16);
}

//get X which should be the first token in all parentheses if used
int getX(std::vector<std::string> &tokens, int index){
	return std::stoi(tokens[index+2], nullptr, 16);
}

//get NNN which is always the first token in the parentheses
int getNNN(std::vector<std::string> &tokens, int index) {
	return std::stoi(tokens[index+2], nullptr, 16);
}

void langCompile(std::vector<char> &ROMBytes, std::string outputFile, int returnCode) {
	//we need to loop through all of the bytes of our ROMBytes array and create tokens
	std::vector<int> opcodes; //this just stores all of the completed assembled opcodes
	std::vector<std::string> tokens; //this stores all of the tokens that are created from the simple parsing rules
	std::string token = ""; //current working token
	bool inComment = 0; //denotes whether we are in a comment or not

	//loop through all of the bytes present in the file
	for (int i = 0; i < ROMBytes.size(); i++) {
		if(ROMBytes[i] == '#') inComment = !inComment; //if we find a pound sign we need to set whether we are in a comment or not (this causes all comments to need to be ended with a pound sign as well)

		//if we are not on a special char then we can add it to the token string
		//special chars include - space \n \r \0 : ( ) ,
		if((ROMBytes[i] != ' ' && ROMBytes[i] != '\n' && ROMBytes[i] != '\r' && ROMBytes[i] != '\0' && ROMBytes[i] != ':' && ROMBytes[i] != '(' && ROMBytes[i] != ')' && ROMBytes[i] != ',') && !inComment) //if we do not have a special character then add the char to token
			token += ROMBytes[i];
		else { //if we have a special char then we need to add our token to the list then push the special char to the tokens list
			if(token != "") 
				tokens.push_back(token);
			tokens.push_back(std::string(1, ROMBytes[i]));
			token = "";
		}
	}
	
	//now we need to convert tokens into opcodes
	for (int i = 0; i < tokens.size(); i++) {
		if(tokens[i] == "clear_screen") opcodes.push_back(0x00E0); //00E0
		else if(tokens[i] == "return") opcodes.push_back(0x00EE);  //00EE
		else if (tokens[i] == "goto") opcodes.push_back(0x1000 + getNNN(tokens, i)); //1NNN
		else if(tokens[i] == "call") opcodes.push_back(0x2000 + getNNN(tokens, i)); //2NNN
		else if(tokens[i] == "eqSkip") opcodes.push_back(0x3000 + (getX(tokens, i) << 8) + getNN(tokens, i)); //3XNN
		else if(tokens[i] == "neqSkip") opcodes.push_back(0x4000 + (getX(tokens, i) << 8) + getNN(tokens, i)); //4XNN
		else if(tokens[i] == "regEq") opcodes.push_back(0x5000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4)); //5XY0
		else if(tokens[i] == "vxEq") opcodes.push_back(0x6000 + (getX(tokens, i) << 8) + getNN(tokens, i)); //6XNN
		else if(tokens[i] == "vxInc") opcodes.push_back(0x7000 + (getX(tokens, i) << 8) + getNN(tokens, i)); //7XNN
		else if(tokens[i] == "vxSetRegEq") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4)); //8XY0
		else if(tokens[i] == "vxOrVy") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 1); //8XY1
		else if (tokens[i] == "vxAndVy") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 2); //8XY2
		else if (tokens[i] == "vxXorVy") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 3); //8XY3
		else if (tokens[i] == "vxIncVy") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 4); //8XY4
		else if (tokens[i] == "vxDecVy") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 5); //8XY5
		else if (tokens[i] == "vxRightShift") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 6); //8XY6
		else if (tokens[i] == "vxSubVyVx") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 7); //8XY7
		else if (tokens[i] == "vxLeftShift") opcodes.push_back(0x8000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + 0xE); //8XYE
		else if(tokens[i] == "vxNotEqVy") opcodes.push_back(0x9000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4)); //9XY0
		else if(tokens[i] == "setI") opcodes.push_back(0xA000 + getNNN(tokens, i)); //ANNN
		else if(tokens[i] == "setPC") opcodes.push_back(0xB000 + getNNN(tokens, i)); //BNNN
		else if(tokens[i] == "vxRand") opcodes.push_back(0xC000 + (getX(tokens, i) << 8) + getNN(tokens, i)); //CXNN
		else if(tokens[i] == "draw") opcodes.push_back(0xD000 + (getX(tokens, i) << 8) + (getY(tokens, i) << 4) + getN(tokens, i)); //DXYN
		else if(tokens[i] == "keyEq") opcodes.push_back(0xE000 + (getX(tokens, i) << 8) + 0x9E); //EX9E
		else if (tokens[i] == "keyNotEq") opcodes.push_back(0xE000 + (getX(tokens, i) << 8) + 0xA1); //EXA1
		else if(tokens[i] == "vxEqGetDelay") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x07); //FX07
		else if(tokens[i] == "vxEqGetKey") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x0A); //FX0A
		else if (tokens[i] == "delayEqVx") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x15); //FX15
		else if(tokens[i] == "soundEqVx") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x18); //FX18
		else if(tokens[i] == "iIncVx") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x1E); //FX1E
		else if(tokens[i] == "iEqSprite") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x29); //FX29
		else if(tokens[i] == "dumpVxBCD") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x33); //FX33
		else if(tokens[i] == "regDump") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x55); //FX55
		else if(tokens[i] == "regLoad") opcodes.push_back(0xF000 + (getX(tokens, i) << 8) + 0x65); //FX65
	}
	
	//now we write our opcodes to the file
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