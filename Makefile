#talyape123@gmail.com 
CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -fPIC

# Source files
SRCS = main.cpp Board.cpp Settlement.cpp City.cpp Road.cpp Vertex.cpp Tile.cpp Building.cpp RoadSection.cpp Player.cpp Catan.cpp

# Test source files
SRCS_TEST = Test.cpp TestCounter.cpp Board.cpp Tile.cpp Settlement.cpp City.cpp Road.cpp Vertex.cpp Building.cpp RoadSection.cpp Player.cpp Catan.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)
OBJS_TEST = $(SRCS_TEST:.cpp=.o)

# Executables
EXEC = demo
EXEC_TEST = test_catan

# Define all targets
all: $(EXEC) $(EXEC_TEST)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(EXEC_TEST): $(OBJS_TEST)
	$(CXX) $(CXXFLAGS) $(OBJS_TEST) -o $@

# Compile individual source files
main.o: main.cpp Board.hpp Settlement.hpp City.hpp Road.hpp Vertex.hpp Tile.hpp Building.hpp Player.hpp Catan.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Board.o: Board.cpp Board.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Settlement.o: Settlement.cpp Settlement.hpp Building.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
City.o: City.cpp City.hpp Building.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Road.o: Road.cpp Road.hpp RoadSection.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Vertex.o: Vertex.cpp Vertex.hpp Building.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Tile.o: Tile.cpp Tile.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Building.o: Building.cpp Building.hpp Vertex.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
RoadSection.o: RoadSection.cpp RoadSection.hpp Road.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Player.o: Player.cpp Player.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Catan.o: Catan.cpp Catan.hpp Board.hpp Player.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
Test.o: Test.cpp Board.hpp Player.hpp Settlement.hpp City.hpp RoadSection.hpp Catan_enums.hpp
	$(CXX) $(CXXFLAGS) -c $<
TestCounter.o: TestCounter.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean target
clean:
	rm -rf $(OBJS) $(EXEC) $(OBJS_TEST) $(EXEC_TEST)

# Run target
catan: $(EXEC)
	./$(EXEC)