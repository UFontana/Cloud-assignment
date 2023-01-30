//
// Created by raghu on 12/23/19.
//

#include <iostream>

#include "client.h"
#include "../build/shardkv.grpc.pb.h"

// helper to log errors
void logError(const std::string& method, Status& error) {
    assert(!error.ok());
    std::cerr << "method " << method << " failed with status code "
    << error.error_code() << "\nthe error message was: " << error.error_message() << "\n";
}

void Client::Query() {
    Empty query;
    QueryResponse response;
    ClientContext cc;

    Status status = stub->Query(&cc, query, &response);
    if(status.ok()) {
        // start by resetting config
        configuration.Clear();
        for(const auto& config : response.config()) {
            // now set up shards
            for(const auto& shard : config.shards()) {
                configuration.Insert(config.server(), {shard.lower(), shard.upper()});
            }
        }
    } else {
        logError("Query", status);
    }
}

void Client::Move(const std::string& server, const shard_t &shard) {
    MoveRequest req;
    Empty response;
    ClientContext cc;

    req.set_server(server);
    req.mutable_shard()->set_upper(shard.upper);
    req.mutable_shard()->set_lower(shard.lower);
    Status status = stub->Move(&cc, req, &response);
    if(!status.ok()) {
        logError("Move", status);
    }
}

void Client::Join(const std::string& server) {
    JoinRequest req;
    Empty response;
    ClientContext cc;

    req.set_server(server);
    Status status = stub->Join(&cc, req, &response);
    if(!status.ok()) {
        logError("Join", status);
    }
}

void Client::Leave(const std::vector<std::string>& servers) {
    LeaveRequest req;
    Empty response;
    ClientContext cc;

    for (const std::string& server : servers) {
        req.add_servers(server);
    }

    Status status = stub->Leave(&cc, req, &response);
    if(!status.ok()) {
        logError("Leave", status);
    }
}

void Client::PrintConfig() {
    configuration.Print();
}

void Client::Get(const std::string& key) {
    if (key == "all_users") {
        std::vector<std::string> servers = configuration.AllServers();
        for (std::string server : servers) {
            auto channel = grpc::CreateChannel(server, grpc::InsecureChannelCredentials());
            auto kvStub = Shardkv::NewStub(channel);
            std::cout << "Get server: " << server << "\n";

            ::grpc::ClientContext cc;
            GetRequest req;
            GetResponse res;
            req.set_key(key);

            auto status = kvStub->Get(&cc, req, &res);
            if(status.ok()) {
                std::cout << "Get returned: " << res.data() << "\n";
            } else {
                logError("Get", status);
            }
        }
    } else {
        auto kvStub = getKVStub(key);
        if(kvStub == nullptr) {
            return;
        }

        unsigned int key_id = (unsigned int)extractID(key);
        std::cout << "Get server: " << configuration.GetServer(key_id).value() << "\n";

        ::grpc::ClientContext cc;
        GetRequest req;
        GetResponse res;
        req.set_key(key);

        auto status = kvStub->Get(&cc, req, &res);
        if(status.ok()) {
            std::cout << "Get returned: " << res.data() << "\n";
        } else {
            logError("Get", status);
        }
    }
}

void Client::Delete(const std::string& key) {
    auto kvStub = getKVStub(key);
    if(kvStub == nullptr) {
        return;
    }

    unsigned int key_id = (unsigned int)extractID(key);
    std::cout << "Delete server: " << configuration.GetServer(key_id).value() << "\n";

    ::grpc::ClientContext cc;
    DeleteRequest req;
    Empty res;
    req.set_key(key);

    auto status = kvStub->Delete(&cc, req, &res);
    if(status.ok()) {
        std::cout << "Deleted" <<"\n";
    } else {
        logError("Delete", status);
    }
}

void Client::Put(const std::string &key, const std::string &value, const std::string &user_id) {
    auto kvStub = getKVStub(key);
    if(kvStub == nullptr) {
        return;
    }

    unsigned int key_id = (unsigned int)extractID(key);
    std::cout << "Put server: " << configuration.GetServer(key_id).value() << "\n";

    ::grpc::ClientContext cc;
    PutRequest req;
    Empty res;
    req.set_key(key);
    req.set_data(value);
    req.set_user(user_id);

    auto status = kvStub->Put(&cc, req, &res);
     if(!status.ok()) {
        logError("Put", status);
    }
}

void Client::Append(const std::string &key, const std::string &value) {
    auto kvStub = getKVStub(key);
    if(kvStub == nullptr) {
        return;
    }

    unsigned int key_id = (unsigned int)extractID(key);
    std::cout << "Append server: " << configuration.GetServer(key_id).value() << "\n";

    ::grpc::ClientContext cc;
    AppendRequest req;
    Empty res;
    req.set_key(key);
    req.set_data(value);

    auto status = kvStub->Append(&cc, req, &res);
    if(!status.ok()) {
        logError("Append", status);
    }
}

// helper for getting key-value server stubs given a key. returns nullptr on error
std::unique_ptr<Shardkv::Stub> Client::getKVStub(const std::string key) {
    // get servername
    unsigned int key_id = (unsigned int)extractID(key);
    auto addr = configuration.GetServer(key_id);
    if(!addr.has_value()) {
        // not sure how we could get this case UNLESS we have just never run query, so we'll just do that I guess
        // oh I guess this could happen if the key is out of range too... think more about this - can we assume it
        // doesn't happen?
        Query();
        return nullptr;
    }
    auto channel = grpc::CreateChannel(addr.value(), grpc::InsecureChannelCredentials());
    return Shardkv::NewStub(channel);
}
