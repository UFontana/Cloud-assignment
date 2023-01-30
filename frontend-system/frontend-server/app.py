import re
import subprocess
import sys
from time import sleep

import grpc
from flask import Flask, jsonify, request
from flask_cors import CORS  # comment this on deployment
from google.protobuf.empty_pb2 import Empty

from shard_config import ShardConfig
from shardkv_pb2 import AppendRequest, DeleteRequest, GetRequest, PutRequest
from shardkv_pb2_grpc import ShardkvStub
from shardmaster_pb2 import GDPRDeleteRequest
from shardmaster_pb2_grpc import ShardmasterStub

TRIES = 5

def extractId(key):
    ids = re.findall(r"\d+", key)
    if len(ids) != 1:
        print(f"Key {key} has invalid format!")
    else:
        return int(ids[0])


def updateShardConfig(sc, sm_server):
    """
    Updates the shard config to the most recent value.

    Inputs:
    - sc: an outdated shard config
    - sm_server: the shardmaster server
    """
    channel = grpc.insecure_channel(sm_server)
    stub = ShardmasterStub(channel)
    response = stub.Query(Empty())
    sc.updateConfig(response)

def shardmasterGDPRDelete(sm_server, key):
    """
    Sends GDPR Delete request to the shardmaster. 

    Inputs:
    - sm_server: the shardmaster server
    - key: user_<id> key
    """
    channel = grpc.insecure_channel(sm_server)
    stub = ShardmasterStub(channel)
    stub.GDPRDelete(GDPRDeleteRequest(key=key))

def shardkvGet(server, key):
    """
    Helper function to make a get request to a shardkv server.

    Inputs:
    - server: the shardkv server
    - key: the Get request's key

    Raises:
    - grpc.RpcError: if the status is not grpc.StatusCode.OK
    """
    # Connect to server
    channel = grpc.insecure_channel(server)
    stub = ShardkvStub(channel)
    # Send Get request
    response = stub.Get(GetRequest(key=key))
    # retrieve data from response
    return response.data


def shardkvPut(server, key, data, user=None):
    """
    Helper function to make a put request to a shardkv server.

    Inputs:
    - server: the shardkv server
    - key: the Put request's key
    - data: the Put request's data
    - user: the Put request's user; only matters if post, so defaults to None

    Raises:
    - grpc.RpcError: if the status is not grpc.StatusCode.OK
    """
    # Connect to server
    channel = grpc.insecure_channel(server)
    stub = ShardkvStub(channel)
    # Send Put request
    stub.Put(PutRequest(key=key, data=data, user=user))


def shardkvAppend(server, key, data):
    """
    Helper function to make an append request to a shardkv server.

    Inputs:
    - server: the shardkv server
    - key: the Append request's key
    - data: the Append request's data

    Raises:
    - grpc.RpcError: if the status is not grpc.StatusCode.OK
    """
    # Connect to server
    channel = grpc.insecure_channel(server)
    stub = ShardkvStub(channel)
    # Send Append request
    stub.Append(AppendRequest(key=key, data=data))


def shardkvDelete(server, key):
    """
    Helper function to make a delete request to a shardkv server.

    Inputs:
    - server: the shardkv server
    - key: the Delete request's key

    Raises:
    - grpc.RpcError: if the status is not grpc.StatusCode.OK
    """
    # Connect to server
    channel = grpc.insecure_channel(server)
    stub = ShardkvStub(channel)
    # Send Delete request
    stub.Delete(DeleteRequest(key=key))


### FLASK SERVER


app = Flask(__name__, static_url_path="")
CORS(app)  # comment this on deployment

sc = ShardConfig()


@app.route("/query", defaults={"path": ""}, methods=["GET"])
def serve(path):
    print(sc)

    channel = grpc.insecure_channel(app.config.get("shardmaster_location"))
    stub = ShardmasterStub(channel)
    response = stub.Query(Empty())

    return "Query RPC received: {}".format(response)


@app.route("/getAllUsers", methods=["GET"])
def getAllUsers():
    err = None
    for _ in range(TRIES):
        try:
            all_users = []
            for shard_key in sc.config:
                shard = sc.config[shard_key]
                server = shard.server
                data = shardkvGet(server, "all_users")
                if len(data) > 0: 
                    users = list(filter(None, data.split(",")))
                    for user in users:
                        name = shardkvGet(server, user)
                        all_users.append(
                            {"userId": user, "userName": name, "shard": server}
                        )
            return jsonify(
                {"users": [dict(t) for t in {tuple(d.items()) for d in all_users}]}
            )
        except (IndexError, grpc.RpcError) as e:
            err = e
            print("Error encountered in getAllUsers! Updating cache...")
            updateShardConfig(sc, app.config.get("shardmaster_location"))
        sleep(0.1)

    print("Error encountered: ", err)
    return "", 500


@app.route("/addUser", methods=["POST", "GET"])
def addUser():
    try:
        # First, try to read the JSON request; must be of the form
        #  {
        #  userId: user_<id>,
        #  userName: ...
        #  }
        data = request.get_json()
        user_id = data["userId"]
        user_name = data["userName"]
    except IndexError:
        return jsonify("Invalid JSON request format!")

    key_id = extractId(user_id)
    # Then, repeatedly send a Put Request until an OK response
    err = None
    for _ in range(TRIES):
        try:
            server = sc.getShardServer(key_id)
            shardkvPut(server, user_id, user_name)

            # return responsible server
            return jsonify(server)
        # if getShardServer or stub.Put throws an error, cache is outdated, so update and retry
        except (IndexError, grpc.RpcError) as e:
            err = e
            print("Error encountered in addUser! Updating cache...")
            updateShardConfig(sc, app.config.get("shardmaster_location"))
        sleep(0.1)

    print("Error encountered: ", err)
    return "", 500


@app.route("/allUserPosts", methods=["GET"])
def allUserPosts():
    # First, try to read the GET request; must be of the form
    #  {
    #  userId: user_<id>,
    #  }
    user_id = request.args.get("userId")

    all_posts_key = user_id + "_posts"
    # repeatedly send a Get Request until an OK response (i.e. doesn't raise grpc.RpcError)
    err = None
    for _ in range(TRIES):
        try:
            server = sc.getShardServer(extractId(user_id))
            data = shardkvGet(server, all_posts_key)
            err = None
            break
        except (IndexError):
            print("IndexError\n")
            return jsonify({"posts": []})
        except (grpc.RpcError) as e:
            err = e
            print("Error encountered in allUserPosts! Updating cache...")
            updateShardConfig(sc, app.config.get("shardmaster_location"))
        # Sleep for 100ms between queries
        sleep(0.1)
    if err:
        print("Error encountered: ", err)
        return "", 404

    # Extract post ids from list
    post_keys = filter(None, data.split(","))
    post_ids = list(set([extractId(key) for key in post_keys]))

    # for each post, get its content and server
    posts = []
    err = None
    for post_id in post_ids:
        # repeatedly send a Get request until post content is retrieved
        for _ in range(TRIES):
            try:
                server = sc.getShardServer(post_id)
                post_key = f"post_{post_id}"
                post_content = shardkvGet(server, post_key)
                posts.append(
                    {"postId": post_key, "postContent": post_content, "shard": server}
                )
                err = None
                break
            except (IndexError, grpc.RpcError) as e:
                err = e
                print("Error encountered in allUserPosts 2! Updating cache...")
                updateShardConfig(sc, app.config.get("shardmaster_location"))
            # Sleep for 100ms between queries
            sleep(0.1)
        if err:
            print("Error encountered: ", err)
            return "", 500

    return jsonify({"posts": posts})


@app.route("/addPost", methods=["POST", "GET"])
def addPost():
    try:
        # First, try to read the JSON request; must be of the form
        #  {
        #  postId: post_<id>,
        #  postContent: ...,
        #  userId: user_<id>,
        #  }
        data = request.get_json()
        post_id = data["postId"]
        post_content = data["postContent"]
        user_id = data["userId"]
    except IndexError:
        return jsonify("Invalid JSON request format!")

    # Then, repeatedly send a Put Request until an OK response
    err = None
    for _ in range(TRIES):
        try:
            server = sc.getShardServer(extractId(post_id))
            shardkvPut(server, post_id, post_content, user_id)
            # return responsible server
            return jsonify(server)

        # if getShardServer or stub.Put throws an error, cache is outdated, so update and retry
        except (IndexError, grpc.RpcError) as e:
            err = e
            print("Error encountered in addPost! Updating cache...")
            updateShardConfig(sc, app.config.get("shardmaster_location"))
        # Sleep for 100ms between queries
        sleep(0.1)

    print("Error encountered: ", err)
    return ("", 500)


@app.route("/deleteUser", methods=["POST", "GET"])
def deleteUser():
    try:
        # First, try to read the JSON request; must be of the form
        #  {
        #  id: <user_id>,
        #  }
        data = request.get_json()
        key = data["id"]
    except IndexError:
        return jsonify("Invalid JSON request format!")

    try:
        shardmasterGDPRDelete(app.config.get("shardmaster_location"), key)
        return ('', 204) # intentionally empty
    except (IndexError, grpc.RpcError) as e:
        print("Encountered error in GDPRDelete", e)
        return (jsonify(e.details()), 500) # return the error from shardmaster

@app.route("/deletePost", methods=["POST", "GET"])
def deletePost():
    try:
        # First, try to read the JSON request; must be of the form
        #  {
        #  id: <post_id>,
        #  }
        data = request.get_json()
        post_id = data["postId"]
        user_id = data["userId"]

    except IndexError:
        return jsonify("Invalid JSON request format!")

    # repeatedly send Delete request until an OK response
    err = None
    for _ in range(TRIES):
        try:
            post_server = sc.getShardServer(extractId(post_id))
            shardkvDelete(post_server, post_id)
            user_server = sc.getShardServer(extractId(user_id))
            posts = shardkvGet(user_server, user_id + "_posts")
            new_posts = filter(None, [s.strip() for s in posts.split(",")])
            new_posts = ",".join(list(filter(lambda x: x != post_id and x != extractId(post_id), new_posts))) + ","
            shardkvPut(user_server, user_id + "_posts", new_posts)

            return jsonify({"postServer": post_server, "userServer": user_server})
        except (IndexError, grpc.RpcError) as e:
            err = e
            print("Error encountered in deletePost! Updating cache...")
            updateShardConfig(sc, app.config.get("shardmaster_location"))
        # Sleep for 100ms between queries
        sleep(0.1)

    print("Error encountered: ", err)
    return (jsonify(e.details()), 500)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: app.py <SHARDMASTER IP:PORT>\n")
        sys.exit(1)

    app.config["shardmaster_location"] = sys.argv[1]

    updateShardConfig(sc, app.config.get("shardmaster_location"))

    app.run(host="0.0.0.0")
