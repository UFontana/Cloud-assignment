//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_REPLCOMMAND_H
#define SHARDING_REPLCOMMAND_H

#include <string>
#include <vector>
#include <regex>

class ReplCommand {
public:
    virtual bool Matches(const std::string& line) = 0;
    virtual void Handle(const std::string& line) = 0;
    // help text for this command
    virtual void PrintHelpMessage() = 0;
};


#endif //SHARDING_REPLCOMMAND_H
