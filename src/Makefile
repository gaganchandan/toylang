CC = g++
CFLAGS = -g
BUILD = build
SRC = .
BIN = bin

$(BUILD)/ast.o: $(SRC)/ast.cc $(SRC)/ast.hh
	$(CC) $(CFLAGS) -c $(SRC)/ast.cc -o $(BUILD)/ast.o
$(BUILD)/visitor.o: $(SRC)/visitor.cc $(SRC)/visitor.hh $(SRC)/ast.hh
	$(CC) $(CFLAGS) -c $(SRC)/visitor.cc -o $(BUILD)/visitor.o
$(BUILD)/printer.o: $(SRC)/printer.cc $(SRC)/printer.hh $(SRC)/visitor.hh $(SRC)/ast.hh
	$(CC) $(CFLAGS) -c $(SRC)/printer.cc -o $(BUILD)/printer.o
$(BUILD)/env.o: $(SRC)/env.cc $(SRC)/env.hh
	$(CC) $(CFLAGS) -c $(SRC)/env.cc -o $(BUILD)/env.o
$(BUILD)/typechecker.o: $(SRC)/typechecker.cc $(SRC)/typechecker.hh $(SRC)/visitor.hh $(SRC)/ast.hh $(SRC)/env.hh
	$(CC) $(CFLAGS) -c $(SRC)/typechecker.cc -o $(BUILD)/typechecker.o
$(BUILD)/interpreter.o: $(SRC)/interpreter.cc $(SRC)/interpreter.hh $(SRC)/visitor.hh $(SRC)/ast.hh $(SRC)/env.hh
	$(CC) $(CFLAGS) -c $(SRC)/interpreter.cc -o $(BUILD)/interpreter.o
$(BUILD)/main.o: $(SRC)/main.cc $(SRC)/ast.hh $(SRC)/visitor.hh $(SRC)/printer.hh $(SRC)/typechecker.hh $(SRC)/interpreter.hh $(SRC)/env.hh
	$(CC) $(CFLAGS) -c $(SRC)/main.cc -o $(BUILD)/main.o
$(BIN)/program: $(BUILD)/ast.o $(BUILD)/visitor.o $(BUILD)/printer.o $(BUILD)/env.o $(BUILD)/typechecker.o $(BUILD)/interpreter.o $(BUILD)/main.o
	$(CC) $(CFLAGS) $(BUILD)/ast.o $(BUILD)/visitor.o $(BUILD)/printer.o $(BUILD)/env.o $(BUILD)/typechecker.o $(BUILD)/interpreter.o $(BUILD)/main.o -o $(BIN)/program

program: $(BUILD)/ast.o $(BUILD)/visitor.o $(BUILD)/printer.o $(BUILD)/env.o $(BUILD)/typechecker.o $(BUILD)/interpreter.o $(BUILD)/main.o
	$(CC) $(CFLAGS) $(BUILD)/ast.o $(BUILD)/visitor.o $(BUILD)/printer.o $(BUILD)/env.o $(BUILD)/typechecker.o $(BUILD)/interpreter.o $(BUILD)/main.o -o $(BIN)/program


