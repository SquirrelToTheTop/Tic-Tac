COMP=gcc

CFLAGS= -O2 -Wall 
LDFLAGS = 

SDL_CFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --libs)

$(shell mkdir obj)

SRC=src/
OBJ=obj/

EXEC=TicTacToe.exe

# list of files
sources=$(wildcard $(SRC)*.c)
objects_all=$(patsubst $(SRC)%.c, $(OBJ)%.o, $(sources))
objects=$(filter-out $(OBJ)main.o, $(objects_all))

all: $(EXEC)

$(EXEC): $(objects_all)
	$(COMP) -o $(EXEC) $(objects_all) $(LDFLAGS) $(SDL_LDFLAGS)

$(OBJ)main.o: $(SRC)main.c $(objects)
	$(COMP) -c $(SRC)main.c $(CFLAGS) $(SDL_CFLAGS) -o $(OBJ)main.o

$(OBJ)Forest.o: $(SRC)Forest.c
	$(COMP) -c $(SRC)Forest.c $(CFLAGS) $(SDL_CFLAGS) -o $(OBJ)Forest.o

$(OBJ)Game.o: $(SRC)Game.c
	$(COMP) -c $(SRC)Game.c $(CFLAGS) $(SDL_CFLAGS) -o $(OBJ)Game.o

start: $(EXEC)
	./$(EXEC)

clean:
	@rm -v $(EXEC)
	@rm -v $(objects_all)
	@rmdir -v obj