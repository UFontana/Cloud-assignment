//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_JOINCOMMAND_H
#define SHARDING_JOINCOMMAND_H

#include <regex>

#include "../repl/regexcommand.h"
#include "client.h"

class JoinCommand : public RegexCommand {
public:
    // matches: join <server>
    explicit JoinCommand(Client& cl) : RegexCommand("join .*:\\d+"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override;
private:
    Client& client;
};


#endif //SHARDING_JOINCOMMAND_H
