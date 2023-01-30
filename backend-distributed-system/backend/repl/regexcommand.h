//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_REGEXCOMMAND_H
#define SHARDING_REGEXCOMMAND_H

#include <regex>
#include "replcommand.h"

// implementation of ReplCommand that specifies a regex as the Matching method
class RegexCommand : public ReplCommand {
public:
    explicit RegexCommand(const std::string& pattern) : reg(std::regex(pattern)) {}

    bool Matches(const std::string& line) override {
        return std::regex_match(line, reg);
    }

private:
    std::regex reg;
};


#endif //SHARDING_REGEXCOMMAND_H
