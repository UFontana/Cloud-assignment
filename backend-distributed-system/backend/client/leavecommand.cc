//
// Created by raghu on 12/23/19.
//

#include "leavecommand.h"

void LeaveCommand::Handle(const std::string &line) {
    std::vector<std::string> tokens = split(line);
    // get rid of "leave", now only servers are left
    tokens.erase(tokens.begin());
    client.Leave(tokens);
}

void LeaveCommand::PrintHelpMessage() {
    std::cout << "leave <addr1> <addr2> ... <addrN>\nleave tells the shardmaster that the key-value servers on"
                 " the specified addresses are no longer available to hold shards\n";
}
