//
// Created by raghu on 1/1/20.
//

#include "appendcommand.h"
#include "../common/common.h"

using namespace std;

void AppendCommand::Handle(const std::string &line) {
    // TODO this may need to be redone because of whitespace in the data they're trying to add.
    //  for now we will ignore, clean it up later
    vector<string> tokens = split(line);
    assert(tokens.size() >= 3);
    string key = tokens[1];
    string value = tokens[2];
    for(int i = 3; i < tokens.size(); i ++) {
        // assumes single space delimiter for now
        value += " " + tokens[i];
    }

    client.Append(key, value);
}

void AppendCommand::PrintHelpMessage() {
    std::cout << "append <key> <value>\nAppends <value> to whatever <key> is mapped to. If <key> doesn't exist, "
                 "this command is equivalent to put <key> <value>\n";
}
