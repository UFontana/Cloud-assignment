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
const proto = require('./shardmaster_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?grpc.web.ClientOptions} options
 * @constructor
 * @struct
 * @final
 */
proto.ShardmasterClient =
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
proto.ShardmasterPromiseClient =
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
 *   !proto.JoinRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardmaster_Join = new grpc.web.MethodDescriptor(
  '/Shardmaster/Join',
  grpc.web.MethodType.UNARY,
  proto.JoinRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.JoinRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.JoinRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardmasterClient.prototype.join =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardmaster/Join',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Join,
      callback);
};


/**
 * @param {!proto.JoinRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardmasterPromiseClient.prototype.join =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardmaster/Join',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Join);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.LeaveRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardmaster_Leave = new grpc.web.MethodDescriptor(
  '/Shardmaster/Leave',
  grpc.web.MethodType.UNARY,
  proto.LeaveRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.LeaveRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.LeaveRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardmasterClient.prototype.leave =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardmaster/Leave',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Leave,
      callback);
};


/**
 * @param {!proto.LeaveRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardmasterPromiseClient.prototype.leave =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardmaster/Leave',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Leave);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.MoveRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardmaster_Move = new grpc.web.MethodDescriptor(
  '/Shardmaster/Move',
  grpc.web.MethodType.UNARY,
  proto.MoveRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.MoveRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.MoveRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardmasterClient.prototype.move =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardmaster/Move',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Move,
      callback);
};


/**
 * @param {!proto.MoveRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardmasterPromiseClient.prototype.move =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardmaster/Move',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Move);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.google.protobuf.Empty,
 *   !proto.QueryResponse>}
 */
const methodDescriptor_Shardmaster_Query = new grpc.web.MethodDescriptor(
  '/Shardmaster/Query',
  grpc.web.MethodType.UNARY,
  google_protobuf_empty_pb.Empty,
  proto.QueryResponse,
  /**
   * @param {!proto.google.protobuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.QueryResponse.deserializeBinary
);


/**
 * @param {!proto.google.protobuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.QueryResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.QueryResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardmasterClient.prototype.query =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardmaster/Query',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Query,
      callback);
};


/**
 * @param {!proto.google.protobuf.Empty} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.QueryResponse>}
 *     Promise that resolves to the response
 */
proto.ShardmasterPromiseClient.prototype.query =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardmaster/Query',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_Query);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.GDPRDeleteRequest,
 *   !proto.google.protobuf.Empty>}
 */
const methodDescriptor_Shardmaster_GDPRDelete = new grpc.web.MethodDescriptor(
  '/Shardmaster/GDPRDelete',
  grpc.web.MethodType.UNARY,
  proto.GDPRDeleteRequest,
  google_protobuf_empty_pb.Empty,
  /**
   * @param {!proto.GDPRDeleteRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  google_protobuf_empty_pb.Empty.deserializeBinary
);


/**
 * @param {!proto.GDPRDeleteRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.RpcError, ?proto.google.protobuf.Empty)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.google.protobuf.Empty>|undefined}
 *     The XHR Node Readable Stream
 */
proto.ShardmasterClient.prototype.gDPRDelete =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/Shardmaster/GDPRDelete',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_GDPRDelete,
      callback);
};


/**
 * @param {!proto.GDPRDeleteRequest} request The
 *     request proto
 * @param {?Object<string, string>=} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.google.protobuf.Empty>}
 *     Promise that resolves to the response
 */
proto.ShardmasterPromiseClient.prototype.gDPRDelete =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/Shardmaster/GDPRDelete',
      request,
      metadata || {},
      methodDescriptor_Shardmaster_GDPRDelete);
};


module.exports = proto;

