//
// Created by raghu on 1/1/20.
//

#ifndef SHARDING_GETCOMMAND_H
#define SHARDING_GETCOMMAND_H


#include "../repl/regexcommand.h"
#include "client.h"

class GetCommand : public RegexCommand {
public:
    // matches: get <key>
    explicit GetCommand(Client& cl) : RegexCommand("get .+"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override ;
private:
    Client& client;
};


#endif //SHARDING_GETCOMMAND_H
