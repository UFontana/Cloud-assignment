CXX = g++ -std=c++17
CPPFLAGS += `pkg-config --cflags protobuf grpc` -g
SANFLAG += -fsanitize=address
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl -g

ifndef SAN
SAN := $(SANITIZE)
endif
ifeq ($(SAN),1)
CPPFLAGS += $(SANFLAG)
LDFLAGS += $(SANFLAG)
endif

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PKG_CONFIG_PATH=/usr/lib/pkgconfig

PROTOS_PATH = ../protos

SHARD_PROTOS = shardkv.pb.o shardkv.grpc.pb.o
SHARDMASTER_PROTOS = shardmaster.pb.o shardmaster.grpc.pb.o

EXECS = shardkv shardmaster client shardmanager
TESTS = all_ops append missing_keys server_deletes server_joins server_moves server_rejoins shardmaster_complex_moves shardmaster_error_cases shardmaster_join shardmaster_leave shardmaster_rejoin shardmaster_simple_moves kill_primary kill_backup server_rejoins_complete

SHARD_OBJ = ./shardkv_dir
SHARD_SRC = ../shardkv
SHARDMANAGER_OBJ = ./shardkv_manager_dir
SHARDMANAGER_SRC = ../shardkv_manager
SHARDMASTER_OBJ = ./shardmaster_dir
SHARDMASTER_SRC = ../shardmaster
CLIENT_OBJ = ./client_dir
CLIENT_SRC = ../client
COMMON_OBJ = ./common_dir
COMMON_SRC = ../common
CONFIG_OBJ = ./config_dir
CONFIG_SRC = ../config
REPL_OBJ = ./repl_dir
REPL_SRC = ../repl

SHARD_SRCS = $(wildcard $(SHARD_SRC)/*.cc)
SHARD_OBJS = $(patsubst $(SHARD_SRC)/%.cc,$(SHARD_OBJ)/%.o,$(SHARD_SRCS))
SHARDMANAGER_SRCS = $(wildcard $(SHARDMANAGER_SRC)/*.cc)
SHARDMANAGER_OBJS = $(patsubst $(SHARDMANAGER_SRC)/%.cc,$(SHARDMANAGER_OBJ)/%.o,$(SHARDMANAGER_SRCS))
SHARDMASTER_SRCS = $(wildcard $(SHARDMASTER_SRC)/*.cc)
SHARDMASTER_OBJS = $(patsubst $(SHARDMASTER_SRC)/%.cc,$(SHARDMASTER_OBJ)/%.o,$(SHARDMASTER_SRCS))
CLIENT_SRCS = $(wildcard $(CLIENT_SRC)/*.cc)
CLIENT_OBJS = $(patsubst $(CLIENT_SRC)/%.cc,$(CLIENT_OBJ)/%.o,$(CLIENT_SRCS))
COMMON_SRCS = $(wildcard $(COMMON_SRC)/*.cc)
COMMON_OBJS = $(patsubst $(COMMON_SRC)/%.cc,$(COMMON_OBJ)/%.o,$(COMMON_SRCS))
CONFIG_SRCS = $(wildcard $(CONFIG_SRC)/*.cc)
CONFIG_OBJS = $(patsubst $(CONFIG_SRC)/%.cc,$(CONFIG_OBJ)/%.o,$(CONFIG_SRCS))
REPL_SRCS = $(wildcard $(REPL_SRC)/*.cc)
REPL_OBJS = $(patsubst $(REPL_SRC)/%.cc,$(REPL_OBJ)/%.o,$(REPL_SRCS))

SHARDKV_TESTS_SRC = ../tests/shardkv_tests
SHARDMASTER_TESTS_SRC = ../tests/shardmaster_tests
INT_TESTS_SRC = ../tests/integrated_tests
FAULT_TESTS_SRC = ../tests/fault_tolerance_tests
TEST_UTILS_SRC = ../test_utils

SHARDKV_TESTS_OBJ = ./shardkv_tests
SHARDMASTER_TESTS_OBJ = ./shardmaster_tests
INT_TESTS_OBJ = ./integrated_tests
FAULT_TESTS_OBJ = ./fault_tolerance_tests
TEST_UTILS_OBJ = ./test_utils

TEST_DEPENDS = shardkv.grpc.pb.o shardkv.pb.o shardmaster.grpc.pb.o shardmaster.pb.o $(SHARDMANAGER_OBJ)/shardkv_manager.o $(SHARD_OBJ)/shardkv.o $(SHARDMASTER_OBJ)/shardmaster.o $(COMMON_OBJS) $(CONFIG_OBJS) $(TEST_UTILS_OBJ)/test_utils.o

PROTOS_DEST = protos

vpath %.proto $(PROTOS_PATH)

all: $(EXECS)

$(SHARD_OBJ)/%.o: $(SHARD_SRC)/%.cc $(SHARD_SRC)/shardkv.h | $(SHARD_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(SHARDMANAGER_OBJ)/%.o: $(SHARDMANAGER_SRC)/%.cc $(SHARDMANAGER_SRC)/shardkv_manager.h | $(SHARDMANAGER_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(SHARDMASTER_OBJ)/%.o: $(SHARDMASTER_SRC)/%.cc $(SHARDMASTER_SRC)/shardmaster.h| $(SHARDMASTER_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(CONFIG_OBJ)/%.o: $(CONFIG_SRC)/%.cc $(CONFIG_SRC)/config.h | $(CONFIG_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(COMMON_OBJ)/%.o: $(COMMON_SRC)/%.cc $(COMMON_SRC)/common.h | $(COMMON_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(REPL_OBJ)/%.o: $(REPL_SRC)/%.cc $(REPL_SRC)/repl.h | $(REPL_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(CLIENT_OBJ)/%.o: $(CLIENT_SRC)/%.cc | $(CLIENT_OBJ)
	$(CXX) $(CPPFLAGS) -c $< -o $@

./%.o: ./%.cc
	$(CXX) $(CPPFLAGS) -c $^ -o $@

shardkv: shardkv.grpc.pb.o shardkv.pb.o shardmaster.pb.o shardmaster.grpc.pb.o $(SHARD_OBJS) $(COMMON_OBJS) $(CONFIG_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmanager: shardkv.grpc.pb.o shardkv.pb.o shardmaster.pb.o shardmaster.grpc.pb.o $(SHARDMANAGER_OBJS) $(COMMON_OBJS) $(CONFIG_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster: shardmaster.pb.o shardmaster.grpc.pb.o $(SHARDMASTER_OBJS) $(COMMON_OBJS) $(CONFIG_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

client: shardkv.grpc.pb.o shardkv.pb.o shardmaster.pb.o shardmaster.grpc.pb.o $(CLIENT_OBJS) $(COMMON_OBJS) $(CONFIG_OBJS) $(REPL_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

$(TEST_UTILS_OBJ)/%.o: $(TEST_UTILS_SRC)/%.cc
	$(CXX) $(CPPFLAGS) -c $^ -o $@

$(SHARDKV_TESTS_OBJ)/%.o: $(SHARDKV_TESTS_SRC)/%.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(SHARDMASTER_TESTS_OBJ)/%.o: $(SHARDMASTER_TESTS_SRC)/%.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(INT_TESTS_OBJ)/%.o: $(INT_TESTS_SRC)/%.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(FAULT_TESTS_OBJ)/%.o: $(FAULT_TESTS_SRC)/%.cc
	$(CXX) $(CPPFLAGS) -c $< -o $@

all_ops: $(SHARDKV_TESTS_OBJ)/all_ops.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

append: $(SHARDKV_TESTS_OBJ)/append.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

missing_keys: $(SHARDKV_TESTS_OBJ)/missing_keys.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

server_deletes: $(INT_TESTS_OBJ)/server_deletes.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

server_joins: $(INT_TESTS_OBJ)/server_joins.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

server_moves: $(INT_TESTS_OBJ)/server_moves.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

server_rejoins: $(INT_TESTS_OBJ)/server_rejoins.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

kill_primary: $(FAULT_TESTS_OBJ)/kill_primary.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

kill_backup: $(FAULT_TESTS_OBJ)/kill_backup.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

server_rejoins_complete: $(FAULT_TESTS_OBJ)/server_rejoins_complete.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster_complex_moves: $(SHARDMASTER_TESTS_OBJ)/shardmaster_complex_moves.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster_error_cases: $(SHARDMASTER_TESTS_OBJ)/shardmaster_error_cases.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster_join: $(SHARDMASTER_TESTS_OBJ)/shardmaster_join.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster_leave: $(SHARDMASTER_TESTS_OBJ)/shardmaster_leave.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster_rejoin: $(SHARDMASTER_TESTS_OBJ)/shardmaster_rejoin.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

shardmaster_simple_moves: $(SHARDMASTER_TESTS_OBJ)/shardmaster_simple_moves.o $(TEST_DEPENDS)
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f *.o *.h $(EXECS) $(TESTS) $(SHARD_OBJ)/*.o $(SHARDMASTER_OBJ)/*.o $(SHARDMANAGER_OBJ)/*.o $(COMMON_OBJ)/*.o $(CONFIG_OBJ)/*.o $(REPL_OBJ)/*.o $(CLIENT_OBJ)/*.o
	rm -f *.o *.h $(TEST_UTILS_OBJ)/*.o $(INT_TESTS_OBJ)/*.o $(SHARDKV_TESTS_OBJ)/*.o $(SHARDMASTER_TESTS_OBJ)/*.o $(FAULT_TESTS_OBJ)/*.o

check: $(EXECS) $(TEST_DEPENDS)
	./test.sh

# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.
PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/protocolbuffers/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif

