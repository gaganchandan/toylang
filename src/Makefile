# ast.cc  ast.hh  ast.hh.gch  astvisitor.cc  astvisitor.hh  astvisitor.hh.gch  main.cc  printvisitor.cc  printvisitor.hh  printvisitor.hh

CC = g++
CFLAGS = -g
BUILD = build
SRC = .
BIN = bin 

$(BUILD)/ast.o: $(SRC)/ast.cc $(SRC)/ast.hh
	$(CC) $(CFLAGS) -c $(SRC)/ast.cc -o $(BUILD)/ast.o
$(BUILD)/astvisitor.o: $(SRC)/astvisitor.cc $(SRC)/astvisitor.hh $(SRC)/ast.hh
	$(CC) $(CFLAGS) -c $(SRC)/astvisitor.cc -o $(BUILD)/astvisitor.o
$(BUILD)/printvisitor.o: $(SRC)/printvisitor.cc $(SRC)/printvisitor.hh $(SRC)/astvisitor.hh $(SRC)/ast.hh
	$(CC) $(CFLAGS) -c $(SRC)/printvisitor.cc -o $(BUILD)/printvisitor.o
$(BUILD)/main.o: $(SRC)/main.cc $(SRC)/ast.hh $(SRC)/astvisitor.hh $(SRC)/printvisitor.hh
	$(CC) $(CFLAGS) -c $(SRC)/main.cc -o $(BUILD)/main.o

$(BIN)/program: $(BUILD)/ast.o $(BUILD)/astvisitor.o $(BUILD)/printvisitor.o $(BUILD)/main.o
	$(CC) $(CFLAGS) $(BUILD)/ast.o $(BUILD)/astvisitor.o $(BUILD)/printvisitor.o $(BUILD)/main.o -o $(BIN)/program
