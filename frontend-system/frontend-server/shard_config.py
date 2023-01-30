from collections import namedtuple

from sortedcontainers import SortedDict

# Define struct for a shard
Shard = namedtuple("Shard", ("lower", "server"))


class ShardConfig:
    """
    Basic Shard Configuration cache to quickly retrieve a key's shardkv server and cache the current
    configuration. We use a SortedDict for logarithmic search (as opposed to the built-in dict's
    linear search).

    Initializing a ShardConfig object creates an empty store; use updateConfig with every query to
    the Shardmaster to update the cache, and getShardServer to retrieve the responsible server.
    """

    def __init__(self):
        self.config = SortedDict()

    def __repr__(self):
        config_str = "Shard Config: [\n"
        for shard_key in self.config:
            shard = self.config[shard_key]
            config_str += f"\tShard {shard.server}: [{shard.lower}, {shard_key}]\n"
        config_str += "]\n"
        return config_str

    def updateConfig(self, proto_config):
        """
        Updates the old config to the new one.

        Inputs:
        - proto_config: a shardmaster_pb2.QueryResponse
        """
        self.config.clear()
        for config in proto_config.config:
            for shard in config.shards:
                self.config[shard.upper] = Shard(shard.lower, config.server)

    def getShardServer(self, key_id):
        """
        Retrieves the shardkv server string responsible for the key ID.

        Inputs:
        - key_id: the id of a key

        Returns:
        - the IP:port string (e.g. ubuntu-bionic:9001) of the responsible server, or None if no
        servers are responsible

        Raises:
        - IndexError: if no server is responsible for the ID
        """
        upper = list(self.config.irange(key_id))[0]
        return self.config[upper].server
