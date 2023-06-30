CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/main.o $(OBJ)/CodeList.o $(OBJ)/HuffmanTree.o $(OBJ)/MinList.o
HDRS = $(INC)/CodeList.hpp $(INC)/HuffmanTree.hpp $(INC)/MinList.hpp
CFLAGS = -Wall -c -I$(INC)

EXE = $(BIN)/main

run: $(EXE)
	$(EXE) input.txt compressed.txt -c
	$(EXE) compressed.txt decompressed.txt -d

$(BIN)/main: $(OBJS)
	$(CC) -pg -o $(BIN)/main $(OBJS) $(LIBS)

$(OBJ)/main.o: $(HDRS) $(SRC)/main.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/main.o $(SRC)/main.cpp

$(OBJ)/CodeList.o: $(HDRS) $(SRC)/CodeList.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/CodeList.o $(SRC)/CodeList.cpp

$(OBJ)/HuffmanTree.o: $(HDRS) $(SRC)/HuffmanTree.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/HuffmanTree.o $(SRC)/HuffmanTree.cpp

$(OBJ)/MinList.o: $(HDRS) $(SRC)/MinList.cpp
	$(CC) $(CFLAGS) -o $(OBJ)/MinList.o $(SRC)/MinList.cpp
	
clean:
	rm -f $(EXE) $(OBJS) gmon.out