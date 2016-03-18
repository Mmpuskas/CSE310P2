# Set the compiler
CC := g++
# Set the source code directory
SRC_DIR := src
# Set the test directory
TEST_DIR := test
# Set the compiler command line options
CFLAGS := -Wall -Werror -std=c++11

# Set the list of source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
# We don't want to include main.cc because it would conflict when building for
# the tests
SRC_FILES := $(filter-out $(SRC_DIR)/main.cc, $(SRC_FILES))

# Set the list of test files
TEST_FILES := $(wildcard $(TEST_DIR)/*.cc)
# We don't want to include tester.cc because it never changes and takes a long
# time to compile
TEST_FILES := $(filter-out $(TEST_DIR)/tester.cc, $(TEST_FILES))

# Set the list of header files
INC_DIR := include
HEADERS := $(INC_DIR)/*.h

.PHONY: run clean test

# Run the program
run: program
	./program < thorough.txt

# Run the tests
test: $(TEST_DIR)/tester
	$(TEST_DIR)/tester

# Create the program binary (executable) file
program: $(SRC_FILES) $(HEADERS)
	$(CC) -o program $(SRC_DIR)/main.cc $(SRC_FILES) -I $(INC_DIR) $(CFLAGS)

# Build the test runner
$(TEST_DIR)/tester: $(TEST_DIR)/tester.o $(TEST_FILES) $(SRC_FILES) $(HEADERS)
	$(CC) -o $(TEST_DIR)/tester $(TEST_FILES) $(TEST_DIR)/tester.o \
		$(SRC_FILES) -I $(INC_DIR) $(CFLAGS)

# This builds catch.hpp with a main function
$(TEST_DIR)/tester.o: $(TEST_DIR)/tester.cc
	$(CC) -o $(TEST_DIR)/tester.o -c $(TEST_DIR)/tester.cc $(CFLAGS) \
		-I $(INC_DIR)

# Removes indicated files
clean:
	$(RM) program
	$(RM) $(TEST_DIR)/tester
	$(RM) $(TEST_DIR)/tester.o
