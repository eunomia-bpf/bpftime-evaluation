# CO-RE for eBPF

CO-RE, which stands for "Compile Once - Run Everywhere," is a concept within the eBPF (extended Berkeley Packet Filter) ecosystem. eBPF is a powerful technology that allows for the dynamic insertion of bytecode into the Linux kernel without changing kernel source code or loading kernel modules.

### Understanding CO-RE

1. **Context**: eBPF programs are used for various purposes like networking, security, and performance monitoring. They are often loaded into the kernel to customize its behavior.

2. **Challenge**: Traditionally, eBPF programs are tightly coupled with the kernel version and configuration they were compiled against. This means that an eBPF program compiled for one kernel might not work on a different kernel version or configuration, due to differences in kernel internals, such as data structures.

3. **CO-RE Solution**: CO-RE addresses this challenge by allowing an eBPF program to be compiled once and then run on different kernel versions and configurations without modification. This is achieved through a combination of technologies and techniques:
   - **BTF (BPF Type Format)**: BTF provides rich type information about the kernel data structures, enabling eBPF programs to adapt to different kernel versions.
   - **Relocation**: During the loading of an eBPF program, certain adjustments (relocations) are made based on the BTF information to ensure compatibility with the target kernel's internal structures.
   - **Libbpf**: A userspace library that aids in developing eBPF applications. It handles the complexities of loading and managing eBPF programs and maps, including the relocations needed for CO-RE.

### Why We Need CO-RE

1. **Portability**: Ensures that eBPF programs can run on various kernel versions, enhancing their portability across different systems.

2. **Ease of Deployment**: Simplifies the deployment process of eBPF programs in environments with diverse kernel configurations.

3. **Development Efficiency**: Reduces the need for multiple versions of eBPF programs for different kernel versions, streamlining development and maintenance.

4. **Adaptability**: CO-RE allows eBPF programs to automatically adapt to changes in kernel data structures, making them more resilient to kernel updates.

5. **Performance**: Despite the abstraction, eBPF with CO-RE maintains high performance, crucial for kernel-level operations.

### Example of CO-RE in Userspace

Suppose the host sofeware has a data structure like this:

```c
struct data {
	int a;
	int b;
    int c;
} __attribute__((__packed__));
```

And we write a eBPF program like this, which use c field of data structure, as a plugin of the host software:

```c
int bpf_main(struct data *d)
{
	uint64_t n = print_func(str);
	int x = (int)n + d->c;
	return add_func(x, 12);
}
```

However, in production, the host software has a lot of different versions. the data structure may change in different versions. For example, in another, the data structure is like this:

```c
struct data {
	int a;
	int b;
    int add_field;
    int c;
} __attribute__((__packed__));
```

The CO-RE technology can still ensure the eBPF program can access the c field of data structure correctly, even if the data structure is changed.
