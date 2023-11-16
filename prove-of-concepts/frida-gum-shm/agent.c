#include <fcntl.h>
#include <frida-gum.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 123456
#define SHM_SIZE 1024

static int replacement_open(const char *path, int oflag, ...);

void example_agent_main(const gchar *data, gboolean *stay_resident)
{
	GumInterceptor *interceptor;

	/* We don't want to our library to be unloaded after we return. */
	*stay_resident = TRUE;

	gum_init_embedded();

	g_printerr("example_agent_main(): %s\n", data);

	interceptor = gum_interceptor_obtain();

	/* Transactions are optional but improve performance with multiple
	 * hooks. */
	gum_interceptor_begin_transaction(interceptor);

	gum_interceptor_replace(
		interceptor,
		(gpointer)gum_module_find_export_by_name(NULL, "open"),
		replacement_open, 0x1234, NULL);
	/*
	 * ^
	 * |
	 * This is using replace(), but there's also attach() which can be used
	 * to hook functions without any knowledge of argument types, calling
	 * convention, etc. It can even be used to put a probe in the middle of
	 * a function.
	 */

	gum_interceptor_end_transaction(interceptor);

	int shmid;
	char *memory;

	// Access the shared memory segment
	shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
	if (shmid == -1) {
		perror("shmget");
		return;
	}

	// Attach to the shared memory segment
	memory = (char *)shmat(shmid, NULL, 0);
	if (memory == (char *)-1) {
		perror("shmat");
		return;
	}

	// Read message from the shared memory segment
	printf("Received: %s\n", memory);

	// Detach from the shared memory segment
	if (shmdt(memory) == -1) {
		perror("shmdt");
		return;
	}

	// Remove shared memory segment (optional, but good practice when done)
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		return;
	}
}

static int replacement_open(const char *path, int oflag, ...)
{
	g_printerr("open(\"%s\", 0x%x)\n", path, oflag);
	GumInvocationContext *ctx;
	const char *data;

	ctx = gum_interceptor_get_current_invocation();
	g_assert_nonnull(ctx);

	data = (const char *)gum_invocation_context_get_replacement_data(ctx);
	g_printerr("data: %p\n", data);

	return open(path, oflag);
}
