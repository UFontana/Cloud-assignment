#ifndef SHARDING_SHARDKV_MANAGER_H
#define SHARDING_SHARDKV_MANAGER_H

#include <grpcpp/grpcpp.h>
#include <thread>
#include "../common/common.h"
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <fstream>

#include "../build/shardkv.grpc.pb.h"
#include "../build/shardmaster.grpc.pb.h"


class PingInterval {
    std::chrono::time_point<std::chrono::system_clock> time;
public:
    std::uint64_t GetPingInterval(){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()-time.time_since_epoch()).count();
    };
    void Push(std::chrono::time_point<std::chrono::system_clock> t){
        time = t;
    }
};

class ShardkvManager : public Shardkv::Service {
  using Empty = google::protobuf::Empty;

 public:
  explicit ShardkvManager(std::string addr, const std::string& shardmaster_addr)
      : address(std::move(addr)), sm_address(shardmaster_addr) {
      // TODO: Part 3
      // This thread will query the shardmaster every 1 second for updates
      std::thread heartbeatChecker(
              [this]() {
                  std::chrono::milliseconds timespan(1000);
                  while (shardKV_address.empty()) {
                      std::this_thread::sleep_for(timespan);
                  }

                  while (true){
                    std::this_thread::sleep_for(timespan);
                    PingInterval pp = pingIntervals[this->shardKV_address];
        
                    if(pp.GetPingInterval() > this->deadPingInterval && this->shardKV_address != ""){
                        
                        this->skv_mtx.lock();

                        std::cout << "SEEMS LIKE PRIMARY SERVER DIED" << std::endl;
                        this->shardKV_address = this->shardKV_backup_address;
                        this->shardKV_backup_address = "";
                        this->current_view = this->last_ack_view+1;
                        this->views[this->current_view].push_back(this->shardKV_address);
                        this->views[this->current_view].push_back(this->shardKV_backup_address);

                        this->skv_mtx.unlock();

                    }
                    /*
                    if(bp.GetPingInterval() > this->deadPingInterval && this->shardKV_backup_address != ""){
                        
                        this->skv_mtx.lock();

                        std::cout << "SEEMS LIKE BACKUP SERVER DIED" << std::endl;
                        this->shardKV_backup_address = "";
                        this->current_view = this->last_ack_view+1;
                        this->views[this->current_view].push_back(this->shardKV_address);
                        this->views[this->current_view].push_back(this->shardKV_backup_address);

                        this->skv_mtx.unlock();
                    }
                    */
                  }
              });
      // We detach the thread so we don't have to wait for it to terminate later
      heartbeatChecker.detach();
  };

  // TODO implement these three methods
  ::grpc::Status Get(::grpc::ServerContext* context,
                     const ::GetRequest* request,
                     ::GetResponse* response) override;
  ::grpc::Status Put(::grpc::ServerContext* context,
                     const ::PutRequest* request, Empty* response) override;
  ::grpc::Status Append(::grpc::ServerContext* context,
                        const ::AppendRequest* request,
                        Empty* response) override;
  ::grpc::Status Delete(::grpc::ServerContext* context,
                        const ::DeleteRequest* request,
                        Empty* response) override;
  ::grpc::Status Ping(::grpc::ServerContext* context, const PingRequest* request,
                        ::PingResponse* response) override;

 private:
    // address we're running on (hostname:port)
    const std::string address;

    // shardmaster address
    std::string sm_address;

    // mutex
    std::mutex skv_mtx;

    // address of the primary server
    std::string shardKV_address;

    // address of the backup server
    std::string shardKV_backup_address;

    // current view number
    int64_t current_view = 0;

    // last acknowledged view
    int64_t last_ack_view = 0;

    // the intervals of ping
    std::map<std::string, PingInterval> pingIntervals;

    // views
    std::map<int, std::vector<std::string>> views;

    // time over which consider a server dead
    uint64_t deadPingInterval = 2000;
    // TODO add any fields you want here!
};
#endif  // SHARDING_SHARDKV_MANAGER_H
