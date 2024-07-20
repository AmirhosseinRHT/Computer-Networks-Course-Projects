CXX = g++
CXXFLAGS = -std=c++17 -w
LDFLAGS = 

all: ser cli

ser: server.cpp defs.o
	$(CXX) -pthread $(CXXFLAGS) defs.o  -o ser server.cpp $(LDFLAGS)

Reno.o : Reno.cpp Reno.hpp Client.o
	$(CXX) $(CXXFLAGS) -c Reno.cpp

cli: client.cpp defs.o Client.o Reno.o
	$(CXX) -pthread $(CXXFLAGS) Client.o defs.o Reno.o -o cli client.cpp $(LDFLAGS)


Client.o : Client.hpp defs.o Client.cpp;
	$(CXX) $(CXXFLAGS) -c Client.cpp


defs.o : defs.hpp defs.cpp
	$(CXX) $(CXXFLAGS) -c defs.cpp

clean:
	rm -f ser cli