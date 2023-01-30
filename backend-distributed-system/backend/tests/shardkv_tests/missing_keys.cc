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

  assert(test_join(shardmaster_addr, skv_1, true));
  assert(test_join(shardmaster_addr, skv_2, true));
  assert(test_join(shardmaster_addr, skv_3, true));

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(1000);
  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_1, "user_0", nullopt));
  assert(test_get(skv_2, "post_1", nullopt));
  assert(test_get(skv_3, "user_2", nullopt));

  assert(test_put(skv_1, "user_1", "Bob", "user_1", true));
  assert(test_put(skv_2, "post_500", "hello!", "user_1", true));
  assert(test_get(skv_1, "user_1_posts", "post_500,"));

  assert(test_get(skv_1, "user_1", "Bob"));
  assert(test_get(skv_2, "post_500", "hello!"));

  assert(test_get(skv_3, "user_1", nullopt));
  assert(test_get(skv_3, "post_500", nullopt));

  assert(test_delete(skv_2, "post_500", true));
  assert(test_get(skv_2, "post_500", nullopt));

  assert(test_delete(skv_1, "user_1", true));
  assert(test_get(skv_1, "user_1", nullopt));

  return 0;
}
