COMP=gcc

CFLAGS= -O2 -Wall 
LDFLAGS = 

SDL_CFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --libs)

EXEC=TicTacToe.exe

# list of files
sources=$(wildcard $ *.c)
objects_all=$(patsubst %.c, %.o, $(sources))
objects=$(filter-out main.o, $(objects_all))

all: $(EXEC)

$(EXEC): $(objects_all)
	$(COMP) -o $(EXEC) $(objects_all) $(LDFLAGS) $(SDL_LDFLAGS)

main.o: main.c $(objects)
	$(COMP) -c main.c $(CFLAGS) $(SDL_CFLAGS) -o main.o

start: $(EXEC)
	./$(EXEC)
clean:
	@rm -v $(EXEC)
	@rm -v $(objects_all)
