//
// Created by raghu on 1/1/20.
//

#include "getcommand.h"
#include "../common/common.h"

using namespace std;

void GetCommand::Handle(const std::string &line) {
    vector<string> tokens = split(line);
    assert(tokens.size() == 2);
    string key = tokens[1];
    client.Get(key);
}

void GetCommand::PrintHelpMessage() {
    std::cout << "get <key>\nretrieves the value associated with <key>, prints an error if the key is not found\n";
}
