CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
GTEST_FLAGS := -lgtest -lgtest_main -lpthread

SRC := decoder.cpp main.cpp test_decoder.cpp
OBJ := $(SRC:.cpp=.o)
EXEC_TESTS := test_decoder
EXEC_MAIN := main

all: $(EXEC_TESTS) $(EXEC_MAIN)

$(EXEC_TESTS): test_decoder.o decoder.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(GTEST_FLAGS)

$(EXEC_MAIN): main.o decoder.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp decoder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(EXEC_TESTS)
	./$(EXEC_TESTS)

run: $(EXEC_MAIN)
	./$(EXEC_MAIN)

clean:
	rm -f $(OBJ) $(EXEC_TESTS) $(EXEC_MAIN) *.bin *.txt

.PHONY: all test run clean
