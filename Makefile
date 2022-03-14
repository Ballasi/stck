# SPDX-License-Identifier: GPL-3.0-or-later

BIN = bin
OBJ = obj
SRC = src
NAME = stck
SRC_ABS = $(shell pwd)/$(SRC)

ERROR_FLAGS ?= -Wall -pedantic -Wno-deprecated-declarations -Werror
LINKING_FLAGS ?=
COMPILE_FLAGS ?= -I$(SRC_ABS)
EXECUTION_FLAGS ?=

SOURCES = $(shell find $(SRC) -type f | grep \.c$)
OBJECTS = $(shell find $(SRC) -type f | sed -En 's/^src(.*)\.c/obj\1.o/p')
OBJ_FOLDERS = $(shell find src -type d | sed 's/^src/obj/')

EXAMPLES = $(wildcard examples/*.stck)

run: $(BIN)/$(NAME)
	@printf "\033[1mLaunching $(NAME)...\033[0m\n"
	@env --chdir=$(BIN) ./$(NAME) $(EXECUTION_FLAGS)

$(EXAMPLES): $(BIN)/$(NAME) /tmp/$(NAME)_lock
	@printf "\033[1mRunning example $(shell echo $@ | sed 's/examples\/\(.*\)\.stck/\1/')...\033[0m\n"
	@env --chdir=$(BIN) ./$(NAME) ../$@ $(EXECUTION_FLAGS)

/tmp/$(NAME)_lock:

$(BIN)/$(NAME): $(BIN) $(OBJ_FOLDERS) $(OBJECTS)
	@printf "\033[1mLinking $(NAME)...\033[0m\n"
	@gcc $(OBJECTS) -o $(BIN)/$(NAME) $(LINKING_FLAGS) $(ERROR_FLAGS)

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	@printf "\033[1mCompiling $<...\033[0m\n"
	@gcc -c $< -o $@ $(COMPILE_FLAGS) $(ERROR_FLAGS)

$(OBJ_FOLDERS): $(OBJ)% : $(SRC)%
	@printf "\033[1mCreating $@ folder\033[0m\n"
	@mkdir -p $@

$(BIN):
	@printf "\033[1mCreating $(BIN)/ folder\033[0m\n"
	@mkdir $(BIN)

clean:
	@printf "\033[1mCleaning files\033[0m\n"
	@rm -Rf $(BIN) $(OBJ)
