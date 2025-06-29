# File: Makefile

CXX = g++
# Compiled on O3 level optimization
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -fno-exceptions
TARGET = benchmark
SRC = test.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

