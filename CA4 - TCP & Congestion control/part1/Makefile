CXX = g++
CXXFLAGS = -std=c++17 -w
LDFLAGS = 

all: ser cli

ser: server.cpp defs.hpp
	$(CXX) -pthread $(CXXFLAGS) -o ser server.cpp $(LDFLAGS)

cli: client.cpp defs.hpp
	$(CXX) $(CXXFLAGS) -o cli client.cpp $(LDFLAGS)



clean:
	rm -f ser cli