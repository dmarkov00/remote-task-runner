PRODUCT=TaskRunnerClient

CPPFLAGS=-I../common
CXXFLAGS=-Wall  -Wextra -pedantic -ggdb -O0 $(CPPFLAGS)
CXX=g++

# Paths
OBJ_DIR=obj/
BIN_DIR=bin/
SRC_DIR=src/
CLIENT_SRC_DIR=$(SRC_DIR)client/

OBJ_FILES= $(addprefix $(OBJ_DIR),main.o Client.o)

.PHONY: all create_directories clean

all: create_directories $(BIN_DIR)$(PRODUCT)

create_directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR)

$(BIN_DIR)$(PRODUCT): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

$(OBJ_DIR)Client.o: $(CLIENT_SRC_DIR)Client.cpp $(CLIENT_SRC_DIR)Client.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp $(CLIENT_SRC_DIR)Client.cpp $(CLIENT_SRC_DIR)Client.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
