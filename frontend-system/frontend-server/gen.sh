#!/bin/bash
python3 -m grpc_tools.protoc -I../frontend/protos --python_out=. --grpc_python_out=. ../frontend/protos/shardmaster.proto
python3 -m grpc_tools.protoc -I../frontend/protos --python_out=. --grpc_python_out=. ../frontend/protos/shardkv.proto
