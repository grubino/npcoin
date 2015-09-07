CXX = g++

# :=0 --> UPnP support turned off by default at runtime
# :=1 --> UPnP support turned on by default at runtime
# :=- --> No UPnP support - miniupnp not required
USE_UPNP:=0

# :=1 --> Enable IPv6 support
# :=0 --> Disable IPv6 support
USE_IPV6:=1

LINK:=$(CXX)

LMODE = dynamic

BIN_DIR := $(CURDIR)/bin
INCLUDE_PATH := $(addprefix -I, $(CURDIR)/include)
LIBS += \
   -l boost_system \
   -l boost_serialization \
   -l boost_filesystem \
   -l boost_program_options \
   -l boost_thread \
   -l boost_regex \
   -l boost_log \
   -l sodiumpp

CXXFLAGS += -std=c++11 -O2
LDFLAGS = 

TEST_OBJDIR = src/test-obj
SCRIPT_TEST_OBJ = $(TEST_OBJDIR)/script_io_test.o

CORE_OBJDIR = src/obj
CORE_OBJS = $(addprefix $(CORE_OBJDIR)/, block_chain.o)

SCRIPT_OBJDIR = src/script-obj
SCRIPT_OBJS = $(addprefix $(SCRIPT_OBJDIR)/, grammar.o io.o)

all: test
test: | $(BIN_DIR) $(BIN_DIR)/script_io_test
clean:
	rm -rf $(BIN_DIR) $(SCRIPT_OBJDIR) $(CORE_OBJDIR) $(TEST_OBJDIR)

$(BIN_DIR)/script_io_test: $(SCRIPT_TEST_OBJ) $(SCRIPT_OBJS) $(CORE_OBJS)
	$(LINK) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(SCRIPT_OBJDIR):
	mkdir -p $(SCRIPT_OBJDIR)
$(CORE_OBJDIR):
	mkdir -p $(CORE_OBJDIR)
$(TEST_OBJDIR):
	mkdir -p $(TEST_OBJDIR)

$(SCRIPT_TEST_OBJ): | $(TEST_OBJDIR)
$(TEST_OBJDIR)/script_io_test.o: src/tests/script_io_test.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) -o $@ $<

$(SCRIPT_OBJS): | $(SCRIPT_OBJDIR)
$(SCRIPT_OBJDIR)/%.o: src/script/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) -o $@ $<

$(CORE_OBJS): | $(CORE_OBJDIR)
$(CORE_OBJDIR)/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE_PATH) -o $@ $<
