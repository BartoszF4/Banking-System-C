CC = gcc
CFLAGS = -g -Wall -pedantic
SOURCES = main.c banking.c
HEADERS = banking.h
EXECUTABLE = bank

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) db.dat insurance.dat