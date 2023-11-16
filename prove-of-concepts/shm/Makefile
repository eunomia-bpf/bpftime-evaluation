# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Target executables
TARGETS = writer reader

test: $(TARGETS)
	./writer &
	./reader

writer: shm_writer.c
	$(CC) $(CFLAGS) -o writer shm_writer.c

reader: shm_reader.c
	$(CC) $(CFLAGS) -o reader shm_reader.c

clean:
	rm -f $(TARGETS)
