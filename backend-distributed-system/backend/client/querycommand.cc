//
// Created by raghu on 12/23/19.
//

#include "querycommand.h"

void QueryCommand::Handle(const std::string &line) {
    client.Query();
    client.PrintConfig();
}

void QueryCommand::PrintHelpMessage() {
    std::cout << "query\nqueries the shardmaster for the current configuration of shards and key-value servers\n";
}
