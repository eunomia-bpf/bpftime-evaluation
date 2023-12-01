CC=gcc
CFLAGS=-Wall

fsync_write: fsync_write.c
	$(CC) $(CFLAGS) -o fsync_write fsync_write.c  -luring 

clean:
	rm -f fsync_write fsync_write.o
