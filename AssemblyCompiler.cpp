#include "AssemblyCompiler.h"

void assemblyCompile(std::vector<char> &ROMBytes, std::string outputFile, int returnCode){
    std::vector<std::string> tokens; 
    std::string token = "";
    bool inComment = 0;

    for(int i = 0; i < ROMBytes.size(); i++){
        if(ROMBytes[i] == ';'){
            inComment = true;
            continue;
        }

        if(ROMBytes[i] == '\n' || ROMBytes[i] == '\r' || ROMBytes[i] == '\0'){
            inComment = false;

            if(!token.empty()){
                tokens.push_back(token);
                token = "";
            }

            continue;
        }

        if(ROMBytes[i] != '\0' && ROMBytes[i] != ':' && ROMBytes[i] != ' ' && ROMBytes[i] != ',' && ROMBytes[i] != '\n')
            token += ROMBytes[i];
        else{
            if(token != "")
                tokens.push_back(token);
            tokens.push_back(std::string(1, ROMBytes[i]));
            token = "";
        }
    }

    if(token != ""){
        tokens.push_back(token);
    }

    for(int i = 0; i < tokens.size(); i++){
        std::cout << tokens[i] << std::endl;
    }

}