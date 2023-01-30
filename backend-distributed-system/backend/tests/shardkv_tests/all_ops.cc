#include <unistd.h>
#include <cassert>
#include <optional>
#include <string>

#include "../../test_utils/test_utils.h"

using namespace std;

int main() {
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  string hostname(hostnamebuf);

  string shardmaster_addr = hostname + ":8080";
  start_shardmaster(shardmaster_addr);

  const string skv_1 = hostname + ":8081";
  const string skv_2 = hostname + ":8082";
  const string skv_3 = hostname + ":8083";

  const string sv1 = hostname + ":8001";
  const string sv2 = hostname + ":8002";
  const string sv3 = hostname + ":8003";

  start_shardmanager(skv_1, shardmaster_addr);
  start_shardmanager(skv_2, shardmaster_addr);
  start_shardmanager(skv_3, shardmaster_addr);

  start_shardkvs({sv1}, skv_1);
  start_shardkvs({sv2}, skv_2);
  start_shardkvs({sv3}, skv_3);

  assert(test_join(shardmaster_addr, skv_1, true));
  assert(test_join(shardmaster_addr, skv_2, true));
  assert(test_join(shardmaster_addr, skv_3, true));

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(1000);
  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_1, "user_0", nullopt));
  assert(test_get(skv_2, "user_400", nullopt));
  assert(test_get(skv_3, "user_900", nullopt));
  assert(test_put(skv_3, "user_900", "malte", "", true));

  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_1, "user_900", nullopt));
  assert(test_get(skv_2, "user_900", nullopt));
  assert(test_get(skv_3, "user_900", "malte"));

  assert(test_put(skv_1, "post_0", "cs300!", "user_15", true));
  assert(test_get(skv_1, "post_0", "cs300!"));
  assert(test_delete(skv_1, "post_0", true));
  assert(test_get(skv_1, "post_0", nullopt));

  assert(test_put(skv_2, "user_400", "bob", "", true));
  assert(test_put(skv_2, "post_400", "huh!", "user_400", true));
  assert(test_put(skv_2, "post_401", "abcd", "user_400", true));
  assert(test_get(skv_2, "user_400_posts", "post_400,post_401,"));

  assert(test_put(skv_2, "user_401", "mary", "", true));
  assert(test_get(skv_2, "all_users", "user_400,user_401,"));

  return 0;
}
