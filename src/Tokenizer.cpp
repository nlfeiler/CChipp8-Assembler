#include "Tokenizer.h"

void flushToken(std::vector<std::string> &tokens, std::string &token){
    if(!token.empty()){//flush char
        tokens.push_back(token); //push the token to our tokens vector if it is not empty
        token = ""; //set the current working token to be empty
	}
}


//This function will take the rom input and then tokenize it into tokens that can easily be dealt with in order to figure out what to do with them
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
			flushToken(tokens, token);
			continue; //carry on to next byte in original file
		}

        //if a new line is presented we also need to eat that line and flush the current token as well as mark we are no longer in a comment
        if(ROMBytes[i] == '\n' || ROMBytes[i] == '\r' || ROMBytes[i] == '\0'){
            inComment = false; //no longer in comment if a new line has happened
            flushToken(tokens, token);
            continue; //carry on my wayward son
        }

        //if we are not on a special char and not in a comment then add the current byte to our working token 
        if((ROMBytes[i] != ':' && ROMBytes[i] != ' ' && ROMBytes[i] != ',' && ROMBytes[i] != '\n') && !inComment)
            token += ROMBytes[i];
        else{//if we are on a special char we need to flush token and then push the special char to the tokens
            flushToken(tokens, token);
            tokens.push_back(std::string(1, ROMBytes[i]));
        }
    }

    //push our final token after the loop provided that it is not an empty token
    if(token != "") tokens.push_back(token);
}