//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_REPL_H
#define SHARDING_REPL_H

#include <vector>

#include "replcommand.h"

class Repl {
public:
    void AddCommand(ReplCommand& command);
    void Start();
private:
    // handles a single line of input
    void ProcessLine(const std::string& line);

    std::vector<ReplCommand*> commands;
};


#endif //SHARDING_REPL_H
