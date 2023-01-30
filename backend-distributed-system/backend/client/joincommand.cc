//
// Created by raghu on 12/23/19.
//

#include "joincommand.h"

using namespace std;

void JoinCommand::Handle(const string &line) {
    vector<string> tokens = split(line);
    assert(tokens[0] == "join");
    assert(tokens.size() == 2);
    client.Join(tokens[1]);
}

void JoinCommand::PrintHelpMessage() {
    cout << "join <address>\ntells the shardmaster that a key-value server running on the specified address is "
            " available to distribute shards onto\n";
}
