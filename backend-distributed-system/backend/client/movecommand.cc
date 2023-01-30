//
// Created by raghu on 12/23/19.
//

#include "movecommand.h"

void MoveCommand::Handle(const std::string &line) {
    std::vector<std::string> tokens = split(line);
    unsigned int lower = std::stoi(tokens[2]);
    unsigned int upper = std::stoi(tokens[3]);

    shard_t shard = {lower, upper};
    client.Move(tokens[1], shard);
}

void MoveCommand::PrintHelpMessage() {
    std::cout << "move <addr> <lower> <upper>\nmoves the shard [lower, upper] to "
                 "the key-value server running on <addr>\n";
}
