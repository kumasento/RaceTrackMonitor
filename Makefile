
INCLUDE=include
BIN=bin
SRC=src
UTILDIR=utils

WALL=
DEFINE=

CXX=g++
CC=gcc

main:
	$(CXX) $(WALL) $(DEFINE) \
		$(SRC)/Cache.cpp $(SRC)/RaceTrack.cpp $(SRC)/Monitor.cpp $(SRC)/Scheduler.cpp \
		main.cpp -o main.o \
		-I$(INCLUDE)

addrTrans:
	$(CXX) $(WALL) $(SRC)/Cache.cpp $(UTILDIR)/addrTrans.cpp -o $(BIN)/addrTrans.o -I$(INCLUDE)
idxTrans:
	$(CXX) $(WALL) $(SRC)/Cache.cpp $(UTILDIR)/idxToDim3.cpp -o $(BIN)/idxToDim3.o -I$(INCLUDE)

checkRTGroup:
	$(CXX) $(DEFINE) $(SRC)/RaceTrack.cpp $(UTILDIR)/checkRTGroup.cpp -o $(BIN)/checkRTGroup.o -I$(INCLUDE)
