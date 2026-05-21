#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <algorithm>

int ifLabelGetMemoryLocation(std::vector<std::string> &labels, std::vector<int> &labelMemoryLocations, std::string &token);
void assembleAssemblyFile(std::vector<char> &ROMBytes, std::string outputFile, int &returnCode);
void tokenize(std::vector<char> &ROMBytes, std::vector<std::string> &tokens, std::string outputFile, int &returnCode);
void compile(std::vector<std::string> &tokens, std::vector<int> &opcodes, std::string outputFile, int &returnCode);
void assemble(std::vector<int> &opcodes, std::string outputFile, int &returnCode);