CXX = g++
CFLAGS = -std=c++11 -O2 -Wall
CLIBS = -lpthread

all: global-ht local-ht

global-ht: global-ht.cpp
	$(CXX) $(CFLAGS) global-ht.cpp $(CLIBS) -o global-ht.out

local-ht: local-ht.cpp
	$(CXX) $(CFLAGS) local-ht.cpp $(CLIBS) -o local-ht.out
