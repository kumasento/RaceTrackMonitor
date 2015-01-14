
INCLUDE=include
BIN=bin
SRC=src
UTILDIR=utils

CXX=g++
CC=gcc

main:
	$(CXX) \
		$(SRC)/Cache.cpp $(SRC)/CacheState.cpp \
		main.cpp -o main.o \
		-I$(INCLUDE)

examples:
	$(CXX) $(SRC)/Cache.cpp $(UTILDIR)/addrTrans.cpp -o $(BIN)/addrTrans.o -I$(INCLUDE)
	$(CXX) $(SRC)/Cache.cpp $(UTILDIR)/idxToDim3.cpp -o $(BIN)/idxToDim3.o -I$(INCLUDE)

