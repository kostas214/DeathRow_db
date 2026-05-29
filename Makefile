TITLE = DeathRowBowser


SOURCES = $(TITLE).c str_utils.c


CC = gcc
CFLAGS = -Wall
PROGRAM = $(TITLE)
OBJECTS = $(SOURCES:.c = .o)

all: $(PROGRAM)

$(PROGRAM) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROGRAM)

%.o: %.CC
	$(CC) $(CFLAGS) -c $< -o $@


run: $(PROGRAM)
	./$(PROGRAM)