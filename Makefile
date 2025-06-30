# File: Makefile

CXX = g++
BASE_FLAGS = -std=c++17 -march=native -Wall -Wextra -fno-exceptions
OPT_FLAGS = -O3
DEBUG_FLAGS = -O0

SRC_TEST = test.cpp
SRC_PLOT = plot.cpp
BIN_TEST = benchmark
BIN_PLOT = plot_benchmark
PLOT_SCRIPT = plot_benchmark.py
BENCH_OUT = benchmark.out
PLOT_IMG = benchmark.png

# Check for -o flag to switch optimization
ifeq ($(findstring -o, $(MAKECMDGOALS)), -o)
    CXXFLAGS = $(BASE_FLAGS) $(OPT_FLAGS)
    MAKECMDGOALS := $(filter-out -o, $(MAKECMDGOALS))
else
    CXXFLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS)
endif

.PHONY: all run benchgraph plot clean

# Default build only compiles test.cpp
all: $(BIN_TEST)

$(BIN_TEST): $(SRC_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(BIN_TEST)
	./$(BIN_TEST)

# benchgraph = run full benchmark + graph
benchgraph: $(PLOT_IMG)

$(PLOT_IMG): $(BENCH_OUT) $(PLOT_SCRIPT)
	python3 $(PLOT_SCRIPT)

$(BENCH_OUT): $(BIN_PLOT)
	./$(BIN_PLOT) > $(BENCH_OUT)

$(BIN_PLOT): $(SRC_PLOT)
	$(CXX) $(CXXFLAGS) -o $@ $<

plot: $(PLOT_IMG)
	python3 $(PLOT_SCRIPT)

clean:
	rm -f $(BIN_TEST) $(BIN_PLOT) $(BENCH_OUT) $(PLOT_IMG)

