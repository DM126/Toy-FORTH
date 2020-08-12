CC=g++
FLAGS=-g
OUTFILE=forth.out #name of executable file to create
DEPS = Interpreter.h Parser.h Symbol.h Token.h

#Place object files in a separate directory
OBJ_DIR=obj
OBJ = $(addprefix $(OBJ_DIR)/,driver.o Interpreter.o Parser.o Symbol.o Token.o)

$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	$(CC) $(FLAGS) -c -o $@ $<

all: $(OBJ)
	$(CC) $(FLAGS) $^ -o $(OUTFILE)

#Make the obj directory if none exists
$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o *.out
	rmdir $(OBJ_DIR)
