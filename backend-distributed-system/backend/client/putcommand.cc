//
// Created by raghu on 1/1/20.
//

#include "putcommand.h"
#include "../common/common.h"

using namespace std;

void PutCommand::Handle(const std::string &line) {
    // TODO this may need to be redone because of whitespace in the data they're trying to add.
    //  for now we will ignore, clean it up later
    vector<string> tokens = split(line);
    assert(tokens.size() >= 3);
    if(tokens.size() == 3){
        string user_id = tokens[1];
        string user_name = tokens[2];
        client.Put(user_id, user_name, "");
    }else{
        string key = tokens[1];
        string user_id = tokens[tokens.size()-1];
        string value = tokens[2];
        for(int i = 3; i < tokens.size()-1; i++){
            value += " " + tokens[i];
        }
        client.Put(key, value, user_id);
    }
}

void PutCommand::PrintHelpMessage() {
    cout << "put <key> <value> [user_id]\ninserts a mapping of <key> to <value>, overwriting any previous mapping\n";
}
