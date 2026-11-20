CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
TARGET = json-mini
TEST_TARGET = test_json_mini
SRCS = src/main.cpp src/json_value.cpp
LIB_SRCS = src/json_value.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean sample test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp include/json_mini.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(TEST_TARGET): tests/test_json_mini.cpp $(LIB_SRCS) include/json_mini.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ tests/test_json_mini.cpp $(LIB_SRCS)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

sample: $(TARGET)
	./$(TARGET) --pretty --file samples/example.json

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe $(TEST_TARGET) $(TEST_TARGET).exe
