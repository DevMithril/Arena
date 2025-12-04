CC= gcc
CFLAGS= -Wall -Wextra -pedantic
LDFLAGS= -L../ -Wl,-rpath=../ -larena

EXEC= demo_arena
DEMO_SRC= demo.c
LIBSO= ../libarena.so

run: $(EXEC)
	@./$(EXEC)

all: $(EXEC)

$(EXEC): $(LIBSO) $(DEMO_SRC)
	$(CC) $(CFLAGS) -o $@ $(DEMO_SRC) $(LDFLAGS)

clean:
	@rm -f $(EXEC)