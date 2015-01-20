
INCLUDE=include
BIN=bin
SRC=src
UTILDIR=utils

WALL=-Wdangling-else 

CXX=g++
CC=gcc

main:
	$(CXX) $(WALL) \
		$(SRC)/Cache.cpp $(SRC)/RaceTrack.cpp $(SRC)/Translator.cpp \
		main.cpp -o main.o \
		-I$(INCLUDE)

examples:
	$(CXX) $(WALL) $(SRC)/Cache.cpp $(UTILDIR)/addrTrans.cpp -o $(BIN)/addrTrans.o -I$(INCLUDE)
	$(CXX) $(WALL) $(SRC)/Cache.cpp $(UTILDIR)/idxToDim3.cpp -o $(BIN)/idxToDim3.o -I$(INCLUDE)

