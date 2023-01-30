#include <grpcpp/grpcpp.h>

#include "shardkv.h"

/**
 * This method is analogous to a hashmap lookup. A key is supplied in the
 * request and if its value can be found, we should either set the appropriate
 * field in the response Otherwise, we should return an error. An error should
 * also be returned if the server is not responsible for the specified key
 *
 * @param context - you can ignore this
 * @param request a message containing a key
 * @param response we store the value for the specified key here
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Get(::grpc::ServerContext* context,
                                  const ::GetRequest* request,
                                  ::GetResponse* response) {
  
  std::string key = request->key();
  // std::unique_lock<std::mutex> lock(this->skv_mtx);
  
  if(this->database.find(key)==this->database.end()){
    // lock.unlock();
    return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "Key not found!");
  }

  const std::string &val = database[key];
  response->set_data(val);
  // lock.unlock();
  return ::grpc::Status::OK;
}

/**
 * Insert the given key-value mapping into our store such that future gets will
 * retrieve it
 * If the item already exists, you must replace its previous value.
 * This function should error if the server is not responsible for the specified
 * key.
 *
 * @param context - you can ignore this
 * @param request A message containing a key-value pair
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Put(::grpc::ServerContext* context,
                                  const ::PutRequest* request,
                                  Empty* response) {

    //  I have 4 keys possibilities: 
    //      1. user_id
    //      2. post_id
    //      3. all_users
    //      4. user_id_posts

    //  I can receive key, data and user
    std::string key_req = request->key();
    std::string data_req = request->data();
    std::string user_req = request->user();

    // std::unique_lock<std::mutex> lock(this->skv_mtx);

    // Extract the ID of the key and check if this server is responsible for the managing of it. <Let it do to shardmanager>

    // The first thing, in order to keep consistency, is to update the backup server. This section send the put request to the server
    if(this->primary_address == this->address){

        if (!this->backup_address.empty()){
            
            auto channel = ::grpc::CreateChannel(this->backup_address, ::grpc::InsecureChannelCredentials());
            auto kvStub2 = Shardkv::NewStub(channel);
            ::grpc::ClientContext cc;
            
            auto status = kvStub2->Put(&cc, *request, response);

            if (status.ok()){
                std::cout << "Shardmanager got the Put response from the backup server" << std::endl;
            }else{
                return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "This shardmanager failed");
            }

        }
    }

    // Get the key ID of the input, if is not managed by the server or if the key does not exists, return an invalid argument
    int key_id = extractID(key_req);

    if(this->key_server.find(key_id)==this->key_server.end() || this->key_server[key_id]!=this->shardmanager_address){
        // lock.unlock();
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "Server not responsible for the Key!");
    }

    // If the key does not exists, check: if it is a post and you have the user field then check if this server
    // deals with the current user. If not, communicate with the server that should manage the user and send it the
    // post with the key user_id_posts and value the post_id. It will manage on it's own. Else wait for the response.
    // In any case add then the post to the post_usr map. If you have a user, then append the user to the all_users key.

    if(this->database.find(key_req)==this->database.end()){
        
        // If it is a post request, add it to the user of which post is it. If the user is not present, just
        // put empty string. If the server should not deal with the user, and the server knows which user should
        // deal with that key, send the append request to that server. Else append it to the posts list of the user.
        
        if(key_req.find("post", 0) != std::string::npos){
            
            if(user_req!=""){
                
                int uid = extractID(user_req);
                
                if(this->key_server[uid]==this->shardmanager_address)
                    
                    this->database[user_req+"_posts"] += (key_req+",");
                
                else{
                    
                    std::chrono::milliseconds timespan(100);
                    auto channel = grpc::CreateChannel(key_server[uid], grpc::InsecureChannelCredentials());
                    auto stub = Shardkv::NewStub(channel);
                    
                    std::string user_post_key = user_req + "_posts";

                    // I put a maximum limit for the number of put request that this server can send to the other.
                    // It can be removed.
                    
                    int i = 0;
                    
                    while(i < MAX_TRIAL){
                        
                        ::grpc::ClientContext cc;
                        AppendRequest req;
                        Empty res;
                        
                        req.set_key(user_post_key);
                        req.set_data(key_req);
                        
                        auto stat = stub->Append(&cc, req, &res);
                        if(stat.ok())
                        
                            break;
                        
                        else{
                        
                            std::this_thread::sleep_for(timespan);
                            i++;
                        
                        }
                    }
                    if (i == MAX_TRIAL){
                        // lock.unlock();
                        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "Not possible to contact the right server!");
                    }
                }
            }
            
            this->post_usr[key_req] = user_req;
        }
        else{

            // It is not a post, so add the user to all_users.
            this->database["all_users"] += (key_req+",");
        }
    }
    // Put the key-value inside the database. 
    this->database[key_req] = data_req;

    
    // lock.unlock();
    return ::grpc::Status::OK;
}

/**
 * Appends the data in the request to whatever data the specified key maps to.
 * If the key is not mapped to anything, this method should be equivalent to a
 * put for the specified key and value. If the server is not responsible for the
 * specified key, this function should fail.
 *
 * @param context - you can ignore this
 * @param request A message containngi a key-value pair
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>"
 */
::grpc::Status ShardkvServer::Append(::grpc::ServerContext* context,
                                     const ::AppendRequest* request,
                                     Empty* response) {

    
    // First thing, update the backup server
    
    if(this->primary_address == this->address){

        if(!this->backup_address.empty()){

            auto channel = ::grpc::CreateChannel(this->backup_address, ::grpc::InsecureChannelCredentials());
            auto kvStub = Shardkv::NewStub(channel);
            ::grpc::ClientContext cc;
            
            auto status = kvStub->Append(&cc, *request, response);

            if (status.ok()){
            std::cout << "Shardmanager got the Append response from the backup server" << std::endl;
            }else{

                // lock.unlock();
                return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "This shardmanager failed");
            }
        }
    }

    std::string key = request->key();
    std::string data = request->data();
    
    // std::unique_lock<std::mutex> lock(this->skv_mtx);
    
    int key_id;
    std::string user="";

    // if it is a post, get the user, else if it is user_posts, or all_posts. 
    if(key[0] == 'p')
        user = post_usr[key];
    else if(key[key.length()-1] == 's'){
        this->database[key] += (data+",");
        // lock.unlock();
        return ::grpc::Status::OK;
    }
    else
        user = key;
    
    key_id = extractID(key);
    
    if(this->key_server.find(key_id) == this->key_server.end() || this->key_server[key_id] != this->shardmanager_address){
        // lock.unlock();
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "Server not responsible for the Key!");
    }


    // If it is a post, add it to the user_posts, else add to all users. 
    // If the key doesn't exist, create one. Else, just append the data. 
    if(this->database.find(key) == this->database.end()){

        if(key[0] == 'p'){
            if(user != "")
                this->database[user+"_posts"] += (key+",");
        }
        else
            this->database["all_users"] += (key+",");
        this->database[key] = data;
    }
    else
        this->database[key] += data;
    
    // lock.unlock();
    return ::grpc::Status::OK;
}

/**
 * Deletes the key-value pair associated with this key from the server.
 * If this server does not contain the requested key, do nothing and return
 * the error specified
 *
 * @param context - you can ignore this
 * @param request A message containing the key to be removed
 * @param response An empty message, as we don't need to return any data
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Delete(::grpc::ServerContext* context,
                                           const ::DeleteRequest* request,
                                           Empty* response) {
    std::string key = request->key();
    
    // std::unique_lock<std::mutex> lock(this->skv_mtx);
    
    if(this->database.find(key)!=this->database.end())
        this->database.erase(key);
    else{
        // lock.unlock();
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "Server not responsible for the Key!");
    }
    if(key[0]=='p'){
        if(post_usr[key]!=""){
            int uid = extractID(post_usr[key]);
            if(key_server[uid] == this->shardmanager_address){
                std::string userp = post_usr[key] + "_posts";
                std::string lis = database[userp];
                std::vector<std::string> str_v = parse_value(lis, ",");
                str_v.erase(find(str_v.begin(), str_v.end(), key));
                lis = "";
                for(auto s:str_v){
                    lis += s;
                    lis += ",";
                }
                database[userp] = lis;
            }
        }
        post_usr.erase(key);
    }
    else{
        std::string lis = database["all_users"];
        std::vector<std::string> str_v = parse_value(lis, ",");
        str_v.erase(find(str_v.begin(), str_v.end(), key));
        lis = "";
        for(auto s:str_v){
            lis += s;
            lis += ",";
        }
        database["all_users"] = lis;
    }
    // lock.unlock();
    return ::grpc::Status::OK;
}

/**
 * This method is called in a separate thread on periodic intervals (see the
 * constructor in shardkv.h for how this is done). It should query the shardmaster
 * for an updated configuration of how shards are distributed. You should then
 * find this server in that configuration and look at the shards associated with
 * it. These are the shards that the shardmaster deems this server responsible
 * for. Check that every key you have stored on this server is one that the
 * server is actually responsible for according to the shardmaster. If this
 * server is no longer responsible for a key, you should find the server that
 * is, and call the Put RPC in order to transfer the key/value pair to that
 * server. You should not let the Put RPC fail. That is, the RPC should be
 * continually retried until success. After the put RPC succeeds, delete the
 * key/value pair from this server's storage. Think about concurrency issues like
 * potential deadlock as you write this function!
 *
 * @param stub a grpc stub for the shardmaster, which we use to invoke the Query
 * method!
 */
void ShardkvServer::QueryShardmaster(Shardmaster::Stub* stub) {
    Empty query;
    QueryResponse response;
    ::grpc::ClientContext cc;
    std::chrono::milliseconds timespan(100);

    // First get the configuration from the Shardmaster using the Query function.
    auto status = stub->Query(&cc, query, &response);

    if(status.ok()){
        
        // std::unique_lock<std::mutex> lock(this->skv_mtx);
        
        // QueryResponse contains repeated configurations. All configurations have repeated Shards and a server. 
        for(int i = 0; i < response.config_size(); i++){
            
            // Get the server with the respective shards.
            std::string serv = response.config(i).server();
            
            for(int j = 0; j < response.config(i).shards_size(); j++){
                
                int low = response.config(i).shards(j).lower();
                int up = response.config(i).shards(j).upper();
                
                for(int k = low; k <= up; k++){
                    
                    // if the server is already there (Move)
                    if(key_server.find(k) != key_server.end()){

                        // If this server should not manage the k-th key -> find the server that should from the Configuration, 
                        // send it the key with a put and remove the key from this server.
                        if((serv != key_server[k]) && (key_server[k] == this->shardmanager_address)){
                            
                            auto channel = grpc::CreateChannel(serv, grpc::InsecureChannelCredentials());
                            auto stub = Shardkv::NewStub(channel);

                            std::string usr = "user_" + std::to_string(k);
                            
                            if(database.find(usr) != database.end()){

                                int i = 0;

                                while(i<MAX_TRIAL){
                                    
                                    ::grpc::ClientContext cc;
                                    PutRequest req;
                                    Empty res;
                                    
                                    req.set_key(usr);
                                    req.set_user(usr);
                                    req.set_data(database[usr]);
                                    
                                    auto stat = stub->Put(&cc, req, &res);

                                    if(stat.ok()){
                                        
                                        // remove the user associated with the old key from the data
                                        // and update the all_users entry
                                        this->database.erase(usr);
                                        
                                        std::string lis = database["all_users"];
                                        std::vector<std::string> str_v = parse_value(lis, ",");
                                        
                                        str_v.erase(find(str_v.begin(), str_v.end(), usr));
                                        
                                        lis="";
                                        
                                        for(auto s:str_v){
                                            
                                            lis += s;
                                            lis += ",";
                                        }
                                        // this is the new updated list
                                        database["all_users"] = lis;
                                        break;
                                    }
                                    else{
                                        i++;
                                        std::this_thread::sleep_for(timespan);
                                    }
                                }
                                if (i == MAX_TRIAL){
                                    // lock.unlock();
                                    std::cout << "NOT POSSIBLE TO CONTACT THE SERVER" << std::endl;
                                    exit(1);
                                }
                            }

                            std::string pst = "post_" + std::to_string(k);

                            if(database.find(pst) != database.end()){

                                int i = 0;

                                while(i<MAX_TRIAL){

                                    ::grpc::ClientContext cc;
                                    PutRequest req;
                                    Empty res;
                                    
                                    req.set_key(pst);
                                    req.set_data(database[pst]);
                                    
                                    auto stat = stub->Put(&cc, req, &res);

                                    if(stat.ok()){

                                        this->database.erase(pst);
                                        this->post_usr.erase(pst);
                                        break;

                                    }
                                    else{

                                        i++;
                                        std::this_thread::sleep_for(timespan);
                                    }
                                }
                                if (i == MAX_TRIAL){
                                    // lock.unlock();
                                    std::cout << "NOT POSSIBLE TO CONTACT THE SERVER" << std::endl;
                                    exit(1);
                                }
                            }
                            std::string uip = usr + "_posts";
                            if(database.find(uip) != database.end()){

                                int i = 0;

                                while(i < MAX_TRIAL){
                                    ::grpc::ClientContext cc;
                                    PutRequest req;
                                    Empty res;

                                    req.set_key(uip);
                                    req.set_data(database[uip]);
                                    auto stat = stub->Put(&cc, req, &res);
                                    if(stat.ok()){
                                        this->database.erase(uip);    
                                        break;                                
                                    }
                                    else{
                                        i++;
                                        std::this_thread::sleep_for(timespan);
                                    }
                                }
                                if (i == MAX_TRIAL){
                                    // lock.unlock();
                                    std::cout << "NOT POSSIBLE TO CONTACT THE SERVER" << std::endl;
                                    exit(1);
                                }
                            }
                        }
                    }

                    // Associate with the k-th key the respective server.
                    this->key_server[k] = serv;
                }
            }
        }
        // lock.unlock();
    }
    else{

        exit(1);
    }
}


/**
 * This method is called in a separate thread on periodic intervals (see the
 * constructor in shardkv.h for how this is done).
 * BASIC LOGIC - PART 2
 * It pings the shardmanager to signal the it is alive and available to receive Get, Put, Append and Delete RPCs.
 * The first time it pings the sharmanager, it will  receive the name of the shardmaster to contact (by means of a QuerySharmaster).
 *
 * PART 3
 *
 *
 * @param stub a grpc stub for the shardmaster, which we use to invoke the Query
 * method!
 * */
void ShardkvServer::PingShardmanager(Shardkv::Stub* stub) {
    
    std::unique_lock<std::mutex> lock(this->skv_mtx);

    PingRequest request;
    PingResponse response;
    ::grpc::ClientContext cc;

    request.set_server(this->address);
    request.set_viewnumber(this->viewnumber);

    auto stat = stub->Ping(&cc, request, &response);

    this->viewnumber = response.id();
    this->backup_address = response.backup();
    this->primary_address = response.primary();

    if(stat.ok()){
        if (shardmaster_address.empty()){
            std::string response_address = response.shardmaster();
            this->shardmaster_address.assign(response_address);

            std::cout << "PRIMARY " << this->primary_address << " BACKUP " << this->backup_address << std::endl;
            if (this->primary_address != this->address){
                
                auto channel = grpc::CreateChannel(this->primary_address, grpc::InsecureChannelCredentials());
                auto stub = Shardkv::NewStub(channel);
                ::grpc::ClientContext cc;
                DumpResponse response;
                Empty request;

                auto stat = stub->Dump(&cc, request, &response);
                if (stat.ok()){
                    
                    for( const auto& kv : response.database() )
                        this->database.insert({kv.first, kv.second});

                    std::cout << "Transfert All OK" << std::endl;
                } else{
                    std::cout << "Transfert NOT OK" << std::endl;
                }
            }

        }
    }
    else{
        lock.unlock();
        /*
        std::cout << "Problems with the Shardmanager PING" << std::endl;
        std::cout << "Returned with status " << stat.error_message() << std::endl;
        std::cout << "Got though" << response.primary() << std::endl;
        */
        return;
    }

    

    lock.unlock();

    return; 
}


/**
 * PART 3 ONLY
 *
 * This method is called by a backup server when it joins the system for the firt time or after it crashed and restarted.
 * It allows the server to receive a snapshot of all key-value pairs stored by the primary server.
 *
 * @param context - you can ignore this
 * @param request An empty message
 * @param response the whole database
 * @return ::grpc::Status::OK on success, or
 * ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT, "<your error message
 * here>")
 */
::grpc::Status ShardkvServer::Dump(::grpc::ServerContext* context, const Empty* request, ::DumpResponse* response) {

    std::unique_lock<std::mutex> lock(this->skv_mtx);
    auto dataset = response->mutable_database();
    
    for( const auto& kv : this->database ){
        std::cout << "COPYING " << kv.first << " " << kv.second << std::endl;
        dataset->insert({kv.first, kv.second});
    }

    lock.unlock();
    return ::grpc::Status::OK;
}
