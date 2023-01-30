//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_MOVECOMMAND_H
#define SHARDING_MOVECOMMAND_H


#include "../repl/regexcommand.h"
#include "client.h"

class MoveCommand : public RegexCommand {
public:
    // Matches: move <server> <lower> <upper>
    explicit MoveCommand(Client& cl): RegexCommand("move .*:\\d+ \\d+ \\d+"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override;
private:
    Client& client;
};


#endif //SHARDING_MOVECOMMAND_H
