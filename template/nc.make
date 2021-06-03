CC:=gcc.exe
DIR_SRC:=src_cxx/orig
DIR_OBJ:=bin_cxx
DIR_BIN:=bin_cxx
SRC:=$(wildcard $(DIR_SRC)/*.c)
OBJ:=$(patsubst $(DIR_SRC)/%.c, $(DIR_OBJ)/%.o, $(SRC))
BIN:=$(DIR_BIN)/luac.exe
FLAG_LINK:=-g
FLAG_COMP:=
all: $(BIN)
#
$(BIN): $(OBJ)
	$(CC) $(FLAG_LINK) $(OBJ) -o $@
#
$(OBJ): $(OBJ) $(SRC)
#
$(DIR_BIN)/%.o: $(DIR_SRC)/%.c
	$(CC) -c $(FLAG_COMP) $< -o $@