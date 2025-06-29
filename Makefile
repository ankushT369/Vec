# File: Makefile

CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -fno-exceptions
TARGET = benchmark
SRC = test.cpp
PLOT_SCRIPT = plot_benchmark.py
BENCH_OUT = benchmark.out
PLOT_IMG = benchmark.png

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

$(BENCH_OUT): $(TARGET)
	./$(TARGET) > $(BENCH_OUT)

$(PLOT_IMG): $(BENCH_OUT) $(PLOT_SCRIPT)
	python3 $(PLOT_SCRIPT)

plot: $(PLOT_IMG)

clean:
	rm -f $(TARGET) $(BENCH_OUT) $(PLOT_IMG)

