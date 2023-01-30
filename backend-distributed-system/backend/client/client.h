//
// Created by raghu on 12/23/19.
//

#ifndef SHARDING_CLIENT_H
#define SHARDING_CLIENT_H

#include <grpcpp/grpcpp.h>
#include <map>
#include <vector>

#include "../build/shardmaster.grpc.pb.h"
#include "../build/shardkv.grpc.pb.h"
#include "../common/common.h"
#include "../config/config.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

class Client {
    using Empty = google::protobuf::Empty;
public:
    explicit Client(const std::string& addr) :
        stub(Shardmaster::NewStub(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials()))) {}

    void Query();

    void Join(const std::string& server);

    void Leave(const std::vector<std::string>& servers);

    void Move(const std::string& server, const shard_t& shard);

    void PrintConfig();

    void Get(const std::string& key);

    void Put(const std::string& key, const std::string& value, const std::string& user_id);

    void Append(const std::string& key, const std::string& value);

    void Delete(const std::string& key);

private:
    // helper for getting stubs to shardkv servers given a key
    std::unique_ptr<Shardkv::Stub> getKVStub(const std::string key);

    // grpc stub
    std::unique_ptr<Shardmaster::Stub> stub;

    Config configuration;
};


#endif //SHARDING_CLIENT_H
