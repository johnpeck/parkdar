# Use this compiler.
CC=gcc
# Use these compiler options.
CFLAGS=-std=gnu99

SOURCES=main.c \
		pd_command.c \
		pd_ranger.c \
		pd_usart.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=proto

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@
	
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	rm *.o $(EXECUTABLE)

