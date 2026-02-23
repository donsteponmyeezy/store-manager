# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Isrc

# Source Files
SRC_MAIN = src/main.cpp
SRC_FIFO = src/fifo_queue/fifo_queue.cpp
SRC_PQ   = src/priority_queue/priority_queue.cpp
SRC_SIM  = src/simulation/simulation.cpp

# Target executable name
TARGET = simulation

# Rules
all: $(TARGET)

$(TARGET): $(SRC_MAIN) $(SRC_FIFO) $(SRC_PQ) $(SRC_SIM)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC_MAIN) $(SRC_FIFO) $(SRC_PQ) $(SRC_SIM)

# Clean
clean:
	rm -f $(TARGET)