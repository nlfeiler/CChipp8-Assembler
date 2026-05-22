#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>

int getY(std::vector<std::string>& tokens, int index);
int getX(std::vector<std::string> &tokens, int index);
int getNNN(std::vector<char> &ROMBytes, int index);
void langCompile(std::vector<char> &ROMBytes, std::string outputFile, int returnCode);