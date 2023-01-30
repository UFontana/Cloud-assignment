//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_QUERYCOMMAND_H
#define SHARDING_QUERYCOMMAND_H


#include "../repl/regexcommand.h"
#include "client.h"

class QueryCommand : public RegexCommand {
public:
    explicit QueryCommand(Client& cl) : RegexCommand("query"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override;
private:
    Client& client;
};


#endif //SHARDING_QUERYCOMMAND_H
