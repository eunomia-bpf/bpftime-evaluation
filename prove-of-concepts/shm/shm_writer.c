#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_KEY 123456
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *data;

    // Create shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Attach to the shared memory segment
    data = (char *) shmat(shmid, NULL, 0);
    if (data == (char *)-1) {
        perror("shmat");
        return 1;
    }

    // Write message to the shared memory segment
    strncpy(data, "Hello from writer!", SHM_SIZE);

    // Detach from the shared memory segment
    if (shmdt(data) == -1) {
        perror("shmdt");
        return 1;
    }

    printf("Message written to shared memory. Run the reader program to read it.\n");
    return 0;
}
