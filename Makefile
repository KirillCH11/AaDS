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

test_binary: $(EXEC_MAIN)
	@dd if=/dev/urandom of=test_binary.bin bs=1K count=1 2>/dev/null
	@./$(EXEC_MAIN) test_binary.bin encoded.bin dict.bin
	@[ -f encoded.bin ] && echo "Encoded file created" || echo "Encoded file failed"
	@[ -f dict.bin ] && echo "Dictionary created" || echo "Dictionary failed"
	@[ -s encoded.bin ] && echo "Encoded not empty" || echo "Encoded empty"
	@[ -s dict.bin ] && echo "Dict not empty" || echo "Dict empty"
	@rm -f test_binary.bin encoded.bin dict.bin

clean:
	rm -f $(OBJ) $(EXEC_TESTS) $(EXEC_MAIN) test_encoded.bin test_dict.bin test_input.txt *.bin

.PHONY: all test run test_binary clean
