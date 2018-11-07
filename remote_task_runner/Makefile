PRODUCT=TaskRunner

CPPFLAGS=-I../common -Isrc/tasks_runner/interfaces -Isrc/tasks_runner/tasks/interfaces
CXXFLAGS=-Wall  -Wextra -pedantic -ggdb -O0 $(CPPFLAGS)
CXX=g++
# Paths
OBJ_DIR=obj/
BIN_DIR=bin/
SRC_DIR=src/
SERVER_SRC_DIR=$(SRC_DIR)server/
TASK_RUNNER_DIR=$(SRC_DIR)tasks_runner/
TASKS_DIR=$(TASK_RUNNER_DIR)tasks/

OBJ_FILES= $(addprefix $(OBJ_DIR),main.o Server.o HomeTasksCoordinator.o HomeLight.o TVScreenFileVisualizer.o)

# Testing
TEST_DIR=test/

TEST_BIN_DIR=$(TEST_DIR)bin/
TEST_BIN=$(TEST_BIN_DIR)test_$(PRODUCT)

# Needs to be improved by implementing a recursive wildcard, this works for only for files in /tasks_runner
TEST_SOURCES= $(filter-out src/main.cpp, $(wildcard src/tasks_runner/*.cpp)) \
				$(wildcard src/tasks_runner/*.h) \
				$(wildcard test/*.cpp)

TEST_HEADERS=$(HEADERS) \
			 $(wildcard test/*.h)

LDFLAGS=-lgtest -lgmock -lgmock_main -lpthread


.PHONY: all create_directories clean test

all: create_directories $(BIN_DIR)$(PRODUCT)

create_directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(TEST_BIN_DIR)

clean:
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(TEST_BIN_DIR)

test: create_directories $(TEST_BIN)
	@./$(TEST_BIN) 2> /dev/null


$(BIN_DIR)$(PRODUCT): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp $(SERVER_SRC_DIR)Server.cpp $(SERVER_SRC_DIR)Server.h \
					$(TASK_RUNNER_DIR)HomeTasksCoordinator.cpp $(TASK_RUNNER_DIR)HomeTasksCoordinator.h \
					$(TASKS_DIR)HomeLight.cpp $(TASKS_DIR)HomeLight.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)Server.o: $(SERVER_SRC_DIR)Server.cpp $(SERVER_SRC_DIR)Server.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)HomeTasksCoordinator.o: $(TASK_RUNNER_DIR)HomeTasksCoordinator.cpp $(TASK_RUNNER_DIR)HomeTasksCoordinator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)HomeLight.o: $(TASKS_DIR)HomeLight.cpp $(TASKS_DIR)HomeLight.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)TVScreenFileVisualizer.o: $(TASKS_DIR)TVScreenFileVisualizer.cpp $(TASKS_DIR)TVScreenFileVisualizer.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SOURCES) $(TEST_HEADERS)
	@$(CXX) $(CXXFLAGS) -Itest $(TEST_SOURCES) -o $@ $(LDFLAGS)


