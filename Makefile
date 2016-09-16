TARGET  = popl
SHELL = /bin/bash

CXX      = /usr/bin/g++
CXXFLAGS = -Wall -O3 -Iinclude

OBJ = PoplTest.o
BIN = popl

all:	$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)
	strip $(BIN)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJ) *~

