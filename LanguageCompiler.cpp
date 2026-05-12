#include "LanguageCompiler.h"

//this array actually serves no purpose but is here just in case for easy access to mnemonics in the code without consulting other documentation
std::string langCodes[35] = {
	"clear_screen", "return", "goto", "call", "eqSkip", "neqSkip", "regEq", "vxEq", "vxInc", "vxSetRegEq", "vxOrVy", "vxAndVy",
	"vxXorVy", "vxIncVy", "vxDecVy", "vxRightShift", "vxSubVyVx", "vxLeftShift", "vxNotEqVy", "setI", "setPC", "vxRand", "draw",
	"keyEq", "keyNotEq", "vxEqGetDelay", "vxEqGetKey", "delayEqVx", "soundEqVx", "iIncVx", "iEqSprite", "dumpVxBCD", "regDump", "regLoad"
};

int getN(std::vector<std::string>& tokens, int index) {
	return std::stoi(tokens[index+6], nullptr, 16);
}

int getNN(std::vector<std::string> &tokens, int index) {
	return std::stoi(tokens[index+4], nullptr, 16);
}

int getY(std::vector<std::string> &tokens, int index) {
	return std::stoi(tokens[index+4], nullptr, 16);
}

int getX(std::vector<std::string> &tokens, int index){
	return std::stoi(tokens[index+2], nullptr, 16);
}

int getNNN(std::vector<std::string> &tokens, int index) {
	return std::stoi(tokens[index+2], nullptr, 16);
}

void langCompile(std::vector<char> &ROMBytes, std::string outputFile, int returnCode) {
	//we need to loop through all of the bytes of our ROMBytes array and create tokens
	std::vector<int> opcodes;
	std::vector<std::string> tokens;
	std::string token = "";
	bool inComment = 0;
	for (int i = 0; i < ROMBytes.size(); i++) {
		if(ROMBytes[i] == '#') inComment = !inComment;

		if((ROMBytes[i] != ' ' && ROMBytes[i] != '\n' && ROMBytes[i] != '\r' && ROMBytes[i] != '\0' && ROMBytes[i] != ':' && ROMBytes[i] != '(' && ROMBytes[i] != ')' && ROMBytes[i] != ',') && !inComment) //if we do not have a special character then add the char to token
			token += ROMBytes[i];
		else { //if we have a special char then we need to add our token to the list then push the special char
			if(token != "")
				tokens.push_back(token);
			tokens.push_back(std::string(1, ROMBytes[i]));
			token = "";
		}
	}
	
	//now we need to convert tokens into opcodes
	for (int i = 0; i < tokens.size(); i++) {
		if(tokens[i] == "clear_screen") opcodes.push_back(0x00E0);
		else if(tokens[i] == "return") opcodes.push_back(0x00EE);
		else if (tokens[i] == "goto") opcodes.push_back(0x1000 + getNNN(tokens, i));
		else if(tokens[i] == "call") opcodes.push_back(0x0000 + getNNN(tokens, i));
		else if(tokens[i] == "eqSkip") opcodes.push_back(0x3000 + (getX(tokens, i) * 100) + getNN(tokens, i));
		else if(tokens[i] == "neqSkip") opcodes.push_back(0x4000 + (getX(tokens, i) * 100) + getNN(tokens, i));
		else if(tokens[i] == "regEq") opcodes.push_back(0x5000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10));
		else if(tokens[i] == "vxEq") opcodes.push_back(0x6000 + (getX(tokens, i) * 100) + getNN(tokens, i));
		else if(tokens[i] == "vxInc") opcodes.push_back(0x7000 + (getX(tokens, i) * 100) + getNN(tokens, i));
		else if(tokens[i] == "vxSetRegEq") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10));
		else if(tokens[i] == "vxOrVy") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 1);
		else if (tokens[i] == "vxAndVy") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 2);
		else if (tokens[i] == "vxXorVy") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 3);
		else if (tokens[i] == "vxIncVy") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 4);
		else if (tokens[i] == "vxDecVy") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 5);
		else if (tokens[i] == "vxRightShift") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 6);
		else if (tokens[i] == "vxSubVyVx") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 7);
		else if (tokens[i] == "vxLeftShift") opcodes.push_back(0x8000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + 0xE);
		else if(tokens[i] == "vxNotEqVy") opcodes.push_back(0x9000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10));
		else if(tokens[i] == "setI") opcodes.push_back(0xA000 + getNNN(tokens, i));
		else if(tokens[i] == "setPC") opcodes.push_back(0xB000 + getNNN(tokens, i));
		else if(tokens[i] == "vxRand") opcodes.push_back(0xC000 + (getX(tokens, i) * 100) + getNN(tokens, i));
		else if(tokens[i] == "draw") opcodes.push_back(0xD000 + (getX(tokens, i) * 100) + (getY(tokens, i) * 10) + getN(tokens, i));
		else if(tokens[i] == "keyEq") opcodes.push_back(0xE000 + (getX(tokens, i) * 100) + 0x9E);
		else if (tokens[i] == "keyNotEq") opcodes.push_back(0xE000 + (getX(tokens, i) * 100) + 0xA1);
		else if(tokens[i] == "vxEqGetDelay") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x07);
		else if(tokens[i] == "vxEqGetKey") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x0A);
		else if (tokens[i] == "delayEqVx") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x15);
		else if(tokens[i] == "soundEqVx") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x18);
		else if(tokens[i] == "iIncVx") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x1E);
		else if(tokens[i] == "iEqSprite") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x29);
		else if(tokens[i] == "dumpVxBCD") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x33);
		else if(tokens[i] == "regDump") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x55);
		else if(tokens[i] == "regLoad") opcodes.push_back(0xF000 + (getX(tokens, i) * 100) + 0x65);
	}
	
	//now we write our opcodes to the file
	try {
		std::ofstream fileOutput(outputFile, std::ios::binary);
		for (int i = 0; i < opcodes.size(); i++) {
			std::cout << "Opcode: " << std::hex << opcodes[i] << std::endl;

			char highByte = (opcodes[i] >> 8) & 0xFF; //now we need to get the high and low byte of the opcode
			char lowByte = opcodes[i] & 0xFF; //get the low byte

			fileOutput.write(&highByte, sizeof(highByte)); //write the high byte
			fileOutput.write(&lowByte, sizeof(lowByte)); //write the low byte
		}
	}
	catch (std::ios_base::failure& e) {//if some error has happened we will tell the user and exit
		std::cout << "Error assembling file. Please try again.\n" << e.what() << std::endl;
		returnCode = -4;
		return;
	}

	std::cout << "Successfully assembled file" << std::endl;
	returnCode = 1;
}