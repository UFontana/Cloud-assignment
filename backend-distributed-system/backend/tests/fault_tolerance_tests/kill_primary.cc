#include <unistd.h>
#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "../../test_utils/test_utils.h"

using namespace std;

class test_exception : public std::exception {
    std::string test_description;
public:
    test_exception(std::string_view test_description) : test_description(test_description) {};
    const char * what() const noexcept override {
        return test_description.c_str();
    }
};

int main() {
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  string hostname(hostnamebuf);

  string shardmaster_addr = hostname + ":8080";
  start_shardmaster(shardmaster_addr);

  string skv_1 = hostname + ":13000";
  string skv_2 = hostname + ":12000";
  //string skv_3 = hostname + ":11000";

  string sv1_primary = hostname + ":13001";
  string sv2_primary = hostname + ":12001";
  //string sv3_1 = hostname + ":11001";

  string sv1_backup = hostname + ":13002";
  string sv2_backup = hostname + ":12002";
  //string sv3_2 = hostname + ":11002";

  start_shardmanager(skv_1, shardmaster_addr);
  start_shardmanager(skv_2, shardmaster_addr);
  //start_shardmanager(skv_3, shardmaster_addr);

  auto pid_shard_1_primary = start_shardkvs_proc({sv1_primary}, skv_1);
  auto pid_shard_2_primary = start_shardkvs_proc({sv2_primary}, skv_2);

  // wait to make sure the primary is set
  std::this_thread::sleep_for(std::chrono::milliseconds{1000});

  auto pid_shard_1_backup = start_shardkvs_proc({sv1_backup}, skv_1);
  auto pid_shard_2_backup = start_shardkvs_proc({sv2_backup}, skv_2);


  std::chrono::milliseconds timespan(5000);
  std::this_thread::sleep_for(timespan);
  map<string, vector<shard_t>> m;
  try {
    if (!test_join(shardmaster_addr, skv_1, true)) {
        throw test_exception("test_join(shardmaster_addr, skv_1, true)");
    }

    m[skv_1].push_back({0, 1000});
    if (!test_query(shardmaster_addr, m)) {
        throw test_exception("test_query(shardmaster_addr, m)");
    }
    m.clear();

    if (!test_join(shardmaster_addr, skv_2, true)) {
        throw test_exception("test_join(shardmaster_addr, skv_2, true)");
    }
    m[skv_1].push_back({0, 500});
    m[skv_2].push_back({501, 1000});

    if (!test_query(shardmaster_addr, m)) {
        throw test_exception("test_query(shardmaster_addr, m)");
    }

    m.clear();

    // sleep to allow shardkvs to query and get initial config
    std::chrono::milliseconds timespan(2000);
    std::this_thread::sleep_for(timespan);

    if (!test_put(skv_1, "post_200", "hello", "user_1", true) ||
        !test_put(skv_1, "post_202", "wow!", "user_1", true) ||
        !test_put(skv_2, "post_600", "hi", "user_2", true) ||
        !test_get(skv_1, "post_200", "hello") ||
        !test_get(skv_1, "post_202", "wow!") ||
        !test_get(skv_2, "post_600", "hi")) {
        throw test_exception("!test_put(skv_1, \"post_200\", \"hello\", \"user_1\", true) ||\n"
                             "        !test_put(skv_1, \"post_202\", \"wow!\", \"user_1\", true) ||\n"
                             "        !test_put(skv_2, \"post_600\", \"hi\", \"user_2\", true) ||\n"
                             "        !test_get(skv_1, \"post_200\", \"hello\") ||\n"
                             "        !test_get(skv_1, \"post_202\", \"wow!\") ||\n"
                             "        !test_get(skv_2, \"post_600\", \"hi\")");
    }

    // kill the primary in shard 1
    //auto ret = kill(pid_shard_1[0], -9);
    //assert(ret==0);

    // now when skv 1 leaves, its keys should transfer to skv 2
    if (!test_leave(shardmaster_addr, {skv_1}, true)) {
        throw test_exception("test_leave(shardmaster_addr, {skv_1}, true)");
    }

    // wait for the transfer
    std::this_thread::sleep_for(timespan);

    // keys not on skv_1
    if (!test_get(skv_1, "post_200", nullopt) ||
        !test_get(skv_1, "post_202", nullopt)) {
        throw test_exception("!test_get(skv_1, \"post_200\", nullopt) ||\n"
                             "        !test_get(skv_1, \"post_202\", nullopt)");
    }

    // all data on skv_2
    if (!test_get(skv_2, "post_200", "hello") ||
        !test_get(skv_2, "post_202", "wow!") ||
        !test_get(skv_2, "post_600", "hi")) {
        throw test_exception("!test_get(skv_2, \"post_200\", \"hello\") ||\n"
                             "        !test_get(skv_2, \"post_202\", \"wow!\") ||\n"
                             "        !test_get(skv_2, \"post_600\", \"hi\")");
    }

    // kill the skv_2 primary
    kill(pid_shard_2_primary[0], SIGKILL);

    std::this_thread::sleep_for(timespan);

    // you should still be able to get the keys
    if (!test_get(skv_2, "post_200", "hello") ||
      !test_get(skv_2, "post_202", "wow!") ||
      !test_get(skv_2, "post_600", "hi")) {
      throw test_exception("!test_get(skv_2, \"post_200\", \"hello\") ||\n"
                           "        !test_get(skv_2, \"post_202\", \"wow!\") ||\n"
                           "        !test_get(skv_2, \"post_600\", \"hi\")");
    }

    // now if skv 1 rejoins, we expect key 600 to end up on it
    if (!test_join(shardmaster_addr, skv_1, true)) {
        throw test_exception("test_join(shardmaster_addr, skv_1, true)");
    }

    // wait for the transfer
    std::this_thread::sleep_for(timespan);

    if (!test_get(skv_2, "post_200", "hello") ||
        !test_get(skv_2, "post_202", "wow!") ||
        !test_get(skv_1, "post_600", "hi")) {
        throw test_exception("!test_get(skv_2, \"post_200\", \"hello\") ||\n"
                             "        !test_get(skv_2, \"post_202\", \"wow!\") ||\n"
                             "        !test_get(skv_1, \"post_600\", \"hi\")");
    }
  } catch (test_exception& e) {
      std::cout << "Test failed: " << std::endl;
      std::cout << e.what() << std::endl;

      for (auto pid : pid_shard_1_primary) {
          kill(pid, SIGKILL);
      }
      for (auto pid : pid_shard_2_primary) {
          kill(pid, SIGKILL);
      }
      for (auto pid : pid_shard_1_backup) {
          kill(pid, SIGKILL);
      }
      for (auto pid : pid_shard_2_backup) {
          kill(pid, SIGKILL);
      }
      assert(true==false);
  }
    for (auto pid : pid_shard_1_primary) {
        kill(pid, SIGKILL);
    }
    for (auto pid : pid_shard_2_primary) {
        kill(pid, SIGKILL);
    }
    for (auto pid : pid_shard_1_backup) {
        kill(pid, SIGKILL);
    }
    for (auto pid : pid_shard_2_backup) {
        kill(pid, SIGKILL);
    }
}
