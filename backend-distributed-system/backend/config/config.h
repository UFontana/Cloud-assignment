//
// Created by raghu on 12/29/19.
//

#ifndef SHARDING_CONFIG_H
#define SHARDING_CONFIG_H

#include <string>
#include <map>
#include <vector>
#include <optional>
#include "../common/common.h"

typedef struct {
    std::string server;
    unsigned int lower;
} ServerAndLower;

class Config {
public:
    // inserts a server and a shard on that server
    void Insert(const std::string& server, const shard_t& shard);

    // retrieves the server currently responsible for the given key. returns none if no such server exists
    std::optional<std::string> GetServer(unsigned int key);

    // returns list of all servers
    std::vector<std::string> AllServers();

    // deletes all entries from the config
    void Clear();

    void Print();
private:
    // we map the upper bound on a shard (i.e shard.upper) to the server that holds it and the lower bound
    // of the shard
    std::map<unsigned int, ServerAndLower> shardToServer;
};


#endif //SHARDING_CONFIG_H
