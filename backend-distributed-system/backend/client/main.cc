//
// Created by raghu on 12/23/19.
//

#include <iostream>
#include <string>

#include "client.h"
#include "../repl/repl.h"
#include "joincommand.h"
#include "querycommand.h"
#include "movecommand.h"
#include "leavecommand.h"
#include "getcommand.h"
#include "appendcommand.h"
#include "putcommand.h"
#include "deletecommand.h"

using namespace std;

int main(int argc, char **argv) {
    // usage is ./client <hostname> <port>
    if(argc != 3) {
        std::cerr << "usage: ./client <hostname> <port>\n";
        return 1;
    }

    const string addr = string(argv[1]) + ":" + string(argv[2]);
    // construct client
    Client client(addr);

    // construct repl and add commands
    Repl repl;
    JoinCommand jc(client);
    repl.AddCommand(jc);
    QueryCommand qc(client);
    repl.AddCommand(qc);
    MoveCommand mc(client);
    repl.AddCommand(mc);
    LeaveCommand lc(client);
    repl.AddCommand(lc);
    GetCommand gc(client);
    repl.AddCommand(gc);
    PutCommand pc(client);
    repl.AddCommand(pc);
    AppendCommand ac(client);
    repl.AddCommand(ac);
    DeleteCommand dc(client);
    repl.AddCommand(dc);

    // now start repl
    repl.Start();

    return 0;
}

