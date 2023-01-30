//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_LEAVECOMMAND_H
#define SHARDING_LEAVECOMMAND_H


#include "../repl/regexcommand.h"
#include "client.h"

class LeaveCommand : public RegexCommand {
public:
    // Match: leave <server1> <server2> ... <serverN>
    explicit LeaveCommand(Client& cl): RegexCommand("leave .*:\\d+( .*:\\d+)*"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override ;
private:
    Client& client;
};


#endif //SHARDING_LEAVECOMMAND_H
