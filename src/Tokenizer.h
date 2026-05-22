#pragma once

#include <vector>
#include <iostream>
#include <string>

void tokenize(std::vector<char> &ROMBytes, std::vector<std::string> &tokens, std::string outputFile, int &returnCode);
void flushToken(std::vector<std::string> &tokens, std::string &token);