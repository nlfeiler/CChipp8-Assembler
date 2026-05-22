#include "Assembler.h"

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