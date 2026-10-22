CC= gcc
CFLAGS= -Wall -Wextra -pedantic
SOFLAGS= -shared -fpic

LIBSO= libarena.so
SRC_DIR= src/
BUID_DIR= build/
BIN_DIR= $(BUID_DIR)bin/
DEP_DIR= $(BUID_DIR)dep/

SRC= $(wildcard $(SRC_DIR)*.c)
OBJ= $(SRC:$(SRC_DIR)%.c=$(BIN_DIR)%.o)
DEP= $(SRC:$(SRC_DIR)%.c=$(DEP_DIR)%.d)

lib: $(LIBSO)

$(LIBSO): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(SOFLAGS)

$(BIN_DIR)%.o: $(SRC_DIR)%.c $(BIN_DIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(DEP_DIR)%.d: $(SRC_DIR)%.c $(DEP_DIR)
	@$(CC) -MM -MT $(@:$(DEP_DIR)%.d=$(BIN_DIR)%.o) -MF $@ $<

%/:
	@mkdir -p $@

clean_all: clean
	@rm -f $(LIBSO)

clean:
	@rm -rf $(BUID_DIR)

-include $(DEP)
