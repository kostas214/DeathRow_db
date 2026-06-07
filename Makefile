
# \
 Skernti Ntisani \
 AM: 2022202500108 \
 dit25108@go.uop.gr \
 \
 Kostantinos Doukas \
 AM: 2022202500036 \
 dit25036@go.uop.gr \


TITLE = main


SOURCES = $(TITLE).c str_utils.c commands.c errors.c report.c


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