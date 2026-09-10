CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
TARGET = json-mini
SRCS = src/main.cpp src/json_value.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean sample

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

src/%.o: src/%.cpp include/json_mini.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

sample: $(TARGET)
	./$(TARGET) --pretty --file samples/example.json

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
