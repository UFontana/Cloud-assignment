//
// Created by raghu on 1/1/20.
//

#include "deletecommand.h"
#include "../common/common.h"

using namespace std;

void DeleteCommand::Handle(const std::string &line) {
    vector<string> tokens = split(line);
    assert(tokens.size() == 2);
    string key = tokens[1];
    client.Delete(key);
}

void DeleteCommand::PrintHelpMessage() {
    std::cout << "del <key>\ndeletes <key> and the associated value, prints an error if the key is not found\n";
}
