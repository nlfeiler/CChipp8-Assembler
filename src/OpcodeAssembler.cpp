#include "OpcodeAssembler.h"

void opcodeAssemble(std::vector<char> &ROMBytes, std::string outputFile, int &returnCode) {
	//if you write your program using the opcodes of the emulator we literally just have to assemble the rom 
	//if the user has written the progam using straight opcodes then there really is no point in using this program 
	//so all that we need is a file stream and just writing the bytes

	std::ofstream fileOutput(outputFile, std::ios::binary); //create our assembled output file
	try {
		for (int i = 0; i < ROMBytes.size(); i += 4) {//loop through all the bytes in the ROM, skip by four because chip8 opcodes are 4 chars
			std::string opcodeString(ROMBytes.begin() + i, ROMBytes.begin() + i + 4); //we need to make our opcode which is just the next 4 chars
			std::stringstream stringStream; //stream to convert opcode to int
			int opcode;

			stringStream << std::hex << opcodeString; //make the string stream have the hex opcode
			stringStream >> opcode; //store the string stream in the opcode 

			std::cout << "Opcode: " << std::hex << opcode << std::endl;

			char highByte = (opcode >> 8) & 0xFF; //now we need to get the high and low byte of the opcode
			char lowByte = opcode & 0xFF; //get the low byte

			fileOutput.write(&highByte, sizeof(highByte)); //write the high byte
			fileOutput.write(&lowByte, sizeof(lowByte)); //write the low byte
		}
	}
	catch (const std::ios_base::failure& e) { //if we have some sort of io error we are going to set the returnCode to -3 and then return
		std::cout << "Error assembling file. Please try again.\n" << e.what() << std::endl;
		returnCode = -3;
		return;
	}
	//file has been successfully assembled so we will now give a return code of 1 and exit this function
	std::cout << outputFile << " has been assembled successfully" << std::endl;
	returnCode = 1;
}