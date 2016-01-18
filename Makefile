TARGET  = popl
SHELL = /bin/bash

CXX     = /usr/bin/g++
CFLAGS  = -Wall -O3 -Iinclude

OBJ = PoplTest.o
BIN = popl

all:	$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)
	strip $(BIN)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJ) *~

