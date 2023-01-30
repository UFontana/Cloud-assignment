//
// Created by raghu on 12/29/19.
//

#include "config.h"

void Config::Print() {
    // guaranteed iteration order, so it doesn't matter how these have been inserted
    for(const auto&[upper, data] : shardToServer) {
        printf("Shard {%d, %d} on server %s\n", data.lower, upper, data.server.c_str());
    }
}

void Config::Insert(const std::string &server, const shard_t &shard) {
    ServerAndLower s = {server, shard.lower};
    shardToServer.emplace(shard.upper, s);
}

std::optional<std::string> Config::GetServer(unsigned int key) {
    auto it = shardToServer.lower_bound(key);
    if(it == shardToServer.end()) {
        return std::nullopt;
    }
    return std::optional<std::string>(it->second.server);
}

std::vector<std::string> Config::AllServers() {
    std::vector<std::string> servers;
    auto it = shardToServer.begin();
    while (it != shardToServer.end()) {
        servers.push_back(it->second.server);
        it++;
    }
    return servers;
}

void Config::Clear() {
    shardToServer.clear();
}
