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

  string skv_1 = hostname + ":11000";
  string sv1 = hostname + ":11001";

  string skv_2 = hostname + ":12000";
  string sv2 = hostname + ":12001";

  string skv_3 = hostname + ":13000";
  string sv3 = hostname + ":13001";

  start_shardmanager(skv_1, shardmaster_addr);
  start_shardmanager(skv_2, shardmaster_addr);
  start_shardmanager(skv_3, shardmaster_addr);

  start_shardkvs({sv1}, skv_1);
  start_shardkvs({sv2}, skv_2);
  start_shardkvs({sv3}, skv_3);

  map<string, vector<shard_t>> m;

  assert(test_join(shardmaster_addr, skv_1, true));
  assert(test_join(shardmaster_addr, skv_2, true));
  assert(test_join(shardmaster_addr, skv_3, true));

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(1000);
  std::this_thread::sleep_for(timespan);

  assert(test_put(skv_1, "user_1", "edith", "", true));
  assert(test_put(skv_1, "user_2", "mary", "", true));
  assert(test_put(skv_1, "user_202", "sybil", "", true));
  assert(test_put(skv_1, "post_200", "hello", "user_1", true));
  assert(test_put(skv_1, "post_202", "wow!", "user_1", true));
  assert(test_put(skv_2, "post_404", "hi", "user_2", true));

  assert(test_get(skv_1, "post_200", "hello"));
  assert(test_get(skv_1, "post_202", "wow!"));
  assert(test_get(skv_2, "post_404", "hi"));
  assert(test_get(skv_1, "all_users", "user_1,user_2,user_202,"));

  // now we move keys onto skv_3
  assert(test_move(shardmaster_addr, skv_3, {201, 404}, true));

  m[skv_1].push_back({0, 200});
  m[skv_2].push_back({405, 667});
  m[skv_3].push_back({201, 404});
  m[skv_3].push_back({668, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  // wait for the transfer
  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_1, "user_1", "edith"));
  assert(test_get(skv_1, "user_2", "mary"));
  assert(test_get(skv_1, "user_202", nullopt));
  assert(test_get(skv_1, "all_users", "user_1,user_2,"));

  assert(test_get(skv_1, "post_200", "hello"));
  assert(test_get(skv_1, "post_202", nullopt));
  assert(test_get(skv_2, "post_404", nullopt));

  assert(test_get(skv_3, "user_202", "sybil"));
  assert(test_get(skv_3, "post_202", "wow!"));
  assert(test_get(skv_3, "post_404", "hi"));
  assert(test_get(skv_3, "all_users", "user_202,"));
}
