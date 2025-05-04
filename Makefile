CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
GTEST_FLAGS := -lgtest -lgtest_main -lpthread

SRC := encoder.cpp main.cpp test_encoder.cpp
OBJ := $(SRC:.cpp=.o)
EXEC_TESTS := test_encoder
EXEC_MAIN := main

all: $(EXEC_TESTS) $(EXEC_MAIN)

$(EXEC_TESTS): test_encoder.o encoder.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(GTEST_FLAGS)

$(EXEC_MAIN): main.o encoder.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp encoder.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(EXEC_TESTS)
	./$(EXEC_TESTS)

run: $(EXEC_MAIN)
	./$(EXEC_MAIN)

clean:
	rm -f $(OBJ) $(EXEC_TESTS) $(EXEC_MAIN) test_encoded.bin test_dict.bin test_input.txt

.PHONY: all test run clean
