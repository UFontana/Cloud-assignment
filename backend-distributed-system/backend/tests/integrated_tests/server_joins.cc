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

  start_shardmanager(skv_1, shardmaster_addr);
  start_shardmanager(skv_2, shardmaster_addr);

  //start_shardkvs({skv_1, skv_2}, shardmaster_addr);
  start_shardkvs({sv1}, skv_1);
  start_shardkvs({sv2}, skv_2);

  map<string, vector<shard_t>> m;

  assert(test_join(shardmaster_addr, skv_1, true));
  m[skv_1].push_back({0, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(5000);
  std::this_thread::sleep_for(timespan);

  assert(test_put(skv_1, "post_600", "hi", "user_1", true));
  assert(test_put(skv_1, "post_200", "hello", "user_5", true));
  assert(test_get(skv_1, "post_600", "hi"));
  assert(test_get(skv_1, "post_200", "hello"));

  // now when a new server joins, "hi" should move to it while "hello" stays
  assert(test_join(shardmaster_addr, skv_2, true));
  m[skv_1].push_back({0, 500});
  m[skv_2].push_back({501, 1000});
  assert(test_query(shardmaster_addr, m));
  m.clear();

  // wait for the key to transfer
  std::this_thread::sleep_for(timespan);
  // key should now be gone from skv_1 and on skv_2
  assert(test_get(skv_1, "post_600", nullopt));
  assert(test_get(skv_1, "post_200", "hello"));
  assert(test_get(skv_2, "post_600", "hi"));
}
