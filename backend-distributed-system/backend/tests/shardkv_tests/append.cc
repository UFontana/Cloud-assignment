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

  const string skv_addr = hostname + ":8081";
  const string sv_addr = hostname + ":8001";

  start_shardmanager(skv_addr, shardmaster_addr);

  start_shardkvs({sv_addr}, skv_addr);

  assert(test_join(shardmaster_addr, skv_addr, true));

  // sleep to allow shardkvs to query and get initial config
  std::chrono::milliseconds timespan(1000);
  std::this_thread::sleep_for(timespan);

  assert(test_get(skv_addr, "user_1_posts", nullopt));
  assert(test_put(skv_addr, "post_0", "hello!", "user_1", true));
  assert(test_get(skv_addr, "user_1_posts", "post_0,"));
  assert(test_put(skv_addr, "post_1", "wow!", "user_1", true));
  assert(test_get(skv_addr, "user_1_posts", "post_0,post_1,"));

  return 0;
}
