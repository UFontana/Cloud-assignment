#include <unistd.h>
#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "../../test_utils/test_utils.h"

using namespace std;

int main() {
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  string hostname(hostnamebuf);

  string shardmaster_addr = hostname + ":8080";
  start_shardmaster(shardmaster_addr);

  string skv_1 = hostname + ":13000";
  string skv_2 = hostname + ":12000";

  string sv1 = hostname + ":13001";
  string sv2 = hostname + ":12001";

  string sv1_backup = hostname + ":13002";
  string sv2_backup = hostname + ":12002";

  start_shardmanager(skv_1, shardmaster_addr);
  start_shardmanager(skv_2, shardmaster_addr);

  start_shardkvs({sv1, sv1_backup}, skv_1);
  start_shardkvs({sv2, sv2_backup}, skv_2);

  map<string, vector<shard_t>> m;

  assert(test_join(shardmaster_addr, skv_1, true));
  m[skv_1].push_back({0, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  assert(test_join(shardmaster_addr, skv_2, true));
  m[skv_1].push_back({0, 500});
  m[skv_2].push_back({501, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(1000);
  std::this_thread::sleep_for(timespan);

  assert(test_put(skv_1, "post_200", "hello", "user_1", true));
  assert(test_put(skv_1, "post_202", "wow!", "user_1", true));
  assert(test_put(skv_2, "post_600", "hi", "user_2", true));

  assert(test_get(skv_1, "post_200", "hello"));
  assert(test_get(skv_1, "post_202", "wow!"));
  assert(test_get(skv_2, "post_600", "hi"));

  // now when skv 1 leaves, its keys should transfer to skv 2
  assert(test_leave(shardmaster_addr, {skv_1}, true));

  // wait for the transfer
  std::this_thread::sleep_for(timespan);

  // keys not on skv_1
  assert(test_get(skv_1, "post_200", nullopt));
  assert(test_get(skv_1, "post_202", nullopt));

  // all data on skv_2
  assert(test_get(skv_2, "post_200", "hello"));
  assert(test_get(skv_2, "post_202", "wow!"));
  assert(test_get(skv_2, "post_600", "hi"));

  // now if skv 1 rejoins, we expect key 600 to end up on it
  assert(test_join(shardmaster_addr, skv_1, true));

  // wait for the transfer
  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_2, "post_200", "hello"));
  assert(test_get(skv_2, "post_202", "wow!"));
  assert(test_get(skv_1, "post_600", "hi"));
}
