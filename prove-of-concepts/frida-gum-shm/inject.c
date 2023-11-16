#include <frida-core.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 123456
#define SHM_SIZE 1024

int main(int argc, char *argv[])
{
	int result = 0;
	const char *path = "./agent.so";
	FridaInjector *injector;
	int pid;
	GError *error;

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
    

	frida_init();

	if (argc != 2)
		goto bad_usage;

	pid = atoi(argv[1]);
	if (pid <= 0)
		goto bad_usage;

	injector = frida_injector_new();
	printf("pid %d\n", pid);

	error = NULL;
	frida_injector_inject_library_file_sync(injector, pid, path,
						"example_agent_main",
						"example data", NULL, &error);
	if (error != NULL) {
		g_printerr("%s\n", error->message);
		g_clear_error(&error);

		result = 1;
	}

	frida_injector_close_sync(injector, NULL, NULL);
	g_object_unref(injector);

	frida_deinit();

	return result;

bad_usage : {
	g_printerr("Usage: %s <pid>\n", argv[0]);
	frida_deinit();
	return 1;
}
}
