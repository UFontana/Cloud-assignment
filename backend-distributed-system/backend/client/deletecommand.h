#ifndef SHARDING_DELETECOMMAND_H
#define SHARDING_DELETECOMMAND_H


#include "../repl/regexcommand.h"
#include "client.h"

class DeleteCommand : public RegexCommand {
public:
    // matches: del <key>
    explicit DeleteCommand(Client& cl) : RegexCommand("del .+"), client(cl) {}
    void Handle(const std::string& line) override;
    void PrintHelpMessage() override ;
private:
    Client& client;
};


#endif //SHARDING_DELETECOMMAND_H
