//
// Created by raghu on 1/1/20.
//

#ifndef SHARDING_APPENDCOMMAND_H
#define SHARDING_APPENDCOMMAND_H

#include "../repl/regexcommand.h"
#include "client.h"

class AppendCommand : public RegexCommand {
public:
    // match: append <key> <value>
    explicit AppendCommand(Client& cl) : RegexCommand("append .+"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override;
private:
    Client& client;
};


#endif //SHARDING_APPENDCOMMAND_H
