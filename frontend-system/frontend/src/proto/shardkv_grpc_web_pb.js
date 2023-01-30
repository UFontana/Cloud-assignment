/**
 * @fileoverview gRPC-Web generated client stub for 
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');


var google_protobuf_empty_pb = require('google-protobuf/google/protobuf/empty_pb.js')
const proto = require('./shardkv_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.ShardkvClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.ShardkvPromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options.format = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GetRequest,
 *   !proto.GetResponse>}
 */
const methodDescriptor_Shardkv_Get = new grpc.web.MethodDescriptor(
  '/Shardkv/Get',
  grpc.web.MethodType.UNARY,
  proto.GetRequest,
  proto.GetResponse,
  /**
   * @param {!proto.GetRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.GetResponse.deserializeBinary
);


/**
 * @param {!proto.GetRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.GetResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.GetResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardkvClient.prototype.get =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardkv/Get',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Get,
      callback);
};


/**
 * @param {!proto.GetRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.GetResponse>}
 *     Promise that resolves to the response
 */
proto.ShardkvPromiseClient.prototype.get =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardkv/Get',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Get);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.PutRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardkv_Put = new grpc.web.MethodDescriptor(
  '/Shardkv/Put',
  grpc.web.MethodType.UNARY,
  proto.PutRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.PutRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.PutRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardkvClient.prototype.put =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardkv/Put',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Put,
      callback);
};


/**
 * @param {!proto.PutRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardkvPromiseClient.prototype.put =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardkv/Put',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Put);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.AppendRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardkv_Append = new grpc.web.MethodDescriptor(
  '/Shardkv/Append',
  grpc.web.MethodType.UNARY,
  proto.AppendRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.AppendRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.AppendRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardkvClient.prototype.append =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardkv/Append',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Append,
      callback);
};


/**
 * @param {!proto.AppendRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardkvPromiseClient.prototype.append =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardkv/Append',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Append);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.DeleteRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardkv_Delete = new grpc.web.MethodDescriptor(
  '/Shardkv/Delete',
  grpc.web.MethodType.UNARY,
  proto.DeleteRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.DeleteRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.DeleteRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardkvClient.prototype.delete =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardkv/Delete',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Delete,
      callback);
};


/**
 * @param {!proto.DeleteRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardkvPromiseClient.prototype.delete =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardkv/Delete',
      request,
      metadata || {},
      methodDescriptor_Shardkv_Delete);
};


module.exports = proto;

