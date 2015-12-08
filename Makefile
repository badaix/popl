VERSION = 0.0.1
TARGET  = popl
SHELL = /bin/bash

CXX     = /usr/bin/g++
CFLAGS  = -Wall -O3 -DVERSION=\"$(VERSION)\"

OBJ = test.o
BIN = popl

all:	$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)
	strip $(BIN)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJ) *~

