//
// Created by raghu on 1/1/20.
//

#ifndef SHARDING_PUTCOMMAND_H
#define SHARDING_PUTCOMMAND_H


#include "../repl/regexcommand.h"
#include "client.h"

class PutCommand : public RegexCommand {
public:
    // match: put <key> <value>
    explicit PutCommand(Client& cl) : RegexCommand("put .+"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override;
private:
    Client& client;
};


#endif //SHARDING_PUTCOMMAND_H
