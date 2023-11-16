#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 123456
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *data;

    // Access the shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
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

    // Read message from the shared memory segment
    printf("Received: %s\n", data);

    // Detach from the shared memory segment
    if (shmdt(data) == -1) {
        perror("shmdt");
        return 1;
    }

    // Remove shared memory segment (optional, but good practice when done)
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        return 1;
    }

    return 0;
}
