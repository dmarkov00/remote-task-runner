PRODUCT=TaskRunner

CPPFLAGS=-I../common
CXXFLAGS=-Wall  -Wextra -pedantic -ggdb -O0 $(CPPFLAGS)
CXX=g++

# Paths
OBJ_DIR=obj/
BIN_DIR=bin/
SRC_DIR=src/
SERVER_SRC_DIR=$(SRC_DIR)server

OBJ_FILES= $(addprefix $(OBJ_DIR),main.o Server.o)

# Testing
#TEST=test_$(PRODUCT)
#
#TEST_SOURCES=$(filter-out product/main.cpp, $(SOURCES)) \
#			 $(wildcard test/*.cpp)
#
#TEST_HEADERS=$(HEADERS) \
#			 $(wildcard test/*.h)
#
#LDFLAGS=-lgtest -lgmock -lgmock_main -lpthread


.PHONY: all create_directories clean test

all: create_directories $(BIN_DIR)$(PRODUCT)

create_directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR)
#$(TEST)

#test: $(TEST)
#	@./$(TEST) 2> /dev/null


$(BIN_DIR)$(PRODUCT): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

$(OBJ_DIR)Server.o: $(SERVER_SRC_DIR)Server.cpp $(SERVER_SRC_DIR)Server.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)main.o: $(SERVER_SRC_DIR)main.cpp $(SERVER_SRC_DIR)Server.cpp $(SERVER_SRC_DIR)Server.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

#$(TEST): $(TEST_SOURCES) $(TEST_HEADERS) Makefile
#	@$(CXX) $(CXXFLAGS) -Itest $(TEST_SOURCES) -o $@ $(LDFLAGS)
#
