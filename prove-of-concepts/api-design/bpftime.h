#ifndef _BPFTIME_RUNTIME_H_
#define _BPFTIME_RUNTIME_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*
 * host related functions. The runtime will be compile into a shared library and
 * injected into the target process.
 */

struct bpftime_config {
	bool jit;
};

// create global bpftime context
void bpftime_init(struct bpftime_config cfg);
// free the global context for runtime
void bpftime_deinit(void);

// register the functions for ffi from btf. ffi can be call dynamically by the
// ffi index at runtime, represents the functions from the host environment.
int bpftime_register_ffi_from_btf(const char *btf_path);

/*
 * object relate functions.The execution environment of the bpf program is
 * determined by the object config.
 */

// ebpf object file
struct bpftime_object;
// open the object elf file and load it into the context
struct bpftime_object *bpftime_object_open(const char *obj_path);
// load btf associate with the host environment
int bpftime_object_load_relocate_btf(const char *btf_path);
// close and free the object
void bpftime_object_close(struct bpftime_object *obj);
// register helper functions.
// helper is called directly by the bpf CALL inst, with a given helper id.
// helper id should be 0-999
int bpftime_object_register_helper(struct bpftime_object *obj,
				   unsigned int index, void *fn);

// load the object into the vm.
// It may contains:
// 1. verify the program
// 2. relocate base on host ffi
// 3. load the program into the vm
// 4. jit compile the program for future use
int bpftime_object_load(struct bpftime_object *obj);
// unload the object from the vm and verify
int bpftime_object_unload(struct bpftime_object *obj);

// The execution unit or bpf function.
struct bpftime_prog;
// find the program by section name
struct bpftime_prog *
bpftime_object_find_program_by_name(struct bpftime_object *obj,
				    const char *name);
struct bpftime_prog *
bpftime_object_find_program_by_id(struct bpftime_object *obj, int index);
struct bpftime_prog *
bpftime_object__next_program(const struct bpftime_object *obj,
			     struct bpftime_prog *prog);

#define bpf_object__for_each_program(pos, obj)                                 \
	for ((pos) = bpftime_object__next_program((obj), NULL); (pos) != NULL; \
	     (pos) = bpftime_object__next_program((obj), (pos)))

const char *bpftime_program__name(const struct bpftime_prog *prog);

// exec userspace bpf vm
uint64_t bpftime_exec_userspace(struct bpftime_prog *prog, void *memory,
				size_t memory_size);

/*
 * attach related API for objects and programs
 */
void *find_function_by_name(const char *name);
// attach to a function in the object. the bpf program will be called before the
// function execution.
int bpftime_program__attach_enter(const struct bpftime_prog *prog,
				  void *function);
// the bpf program will be called after the function execution.
int bpftime_program__attach_exit(const struct bpftime_prog *prog,
				 void *function);
// the bpf program will be called instead of the function execution.
int bpftime_program__replace(const struct bpftime_prog *prog,
				 void *function);

#endif // _EBPF_RUNTIME_H_
