//
// Created by raghu on 12/23/19.
//

#include <iostream>

#include "repl.h"

void Repl::AddCommand(ReplCommand &command) {
    commands.push_back(&command);
}

void Repl::ProcessLine(const std::string& line) {
    for(ReplCommand *c : commands) {
        if(line == "help") {
            c->PrintHelpMessage();
        } else if(c->Matches(line)) {
            c->Handle(line);
            return;
        }
    }

    if(line == "help") {
    	return;
    }

    std::cout << "invalid command: " << line << "\ntype 'help' for a list of commands\n";
}

void Repl::Start() {
    std::cout << "run 'help' for a list of commands\n";
    for(std::string line; getline(std::cin, line);) {
        ProcessLine(line);
    }
}
