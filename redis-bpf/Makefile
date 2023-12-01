CC=gcc
CFLAGS=-Wall

fsync_write: fsync_write.o
	$(CC) $(CFLAGS) -o fsync_write fsync_write.o

fsync_write.o: fsync_write.c
	$(CC) $(CFLAGS) -c fsync_write.c

clean:
	rm -f fsync_write fsync_write.o
