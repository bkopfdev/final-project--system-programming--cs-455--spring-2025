# Spring 2025 System Programming — Final Project

## Written Portion

### 1. Describe what happens to memory released by the OS with regard to virtual memory

When memory is released by the OS (in relation to virtual memory), that block of memory is added to a list of free blocks that can be recycled and used later by future `malloc` calls. What this means is that the program will have a list of garunteed free blocks of memory to pull from so it doesn't need to search a bunch of memory to find a suitable spot for allocation.

---

### 2. In a C program, why does `argv[argc]` return `NULL`?

When you write argv[argc], it will return `NULL` for the very simple reason that it's the standard that argv[argc] is the memory location that holds the `NULL` pointer.

---

### 3. When a system call is made the CPU changes from user mode to kernel mode. Why?

The reason that the CPU changes from user mode to kernel mode is because there are a handful of operations only possible in kernel mode. For example, accessing memory-management hardware and device I/O operations are only possible in kernel mode. The reason that system calls require the CPU to change to kernel mode is because system calls often require the computer to do a "privileged" action. These range from opening and reading from a disk to writing to the network. And these actions are only possible in kernel mode. The reason it is built this way is to enforce security in the system. This process prevents users from having the privilege to kill some processes, or write to protected or reserved memory.

---

### 4. Explain the relationship between the return value of a system call and the global variable `errno`

The relationship between the return value of a system call and `errno` shows itself when there is an error. If the system call has no error, and it succeeds, it will return a positive value. But, if it fails, it will store a negative number in the global variable `errno`, which indivicates a specific error code (or, a specific error in the Linux Error Number Table).

---

### 5. Consider the following C program. The variables and functions get allocated to which memory segments? Write your answers in comments to the right of each item

```c
#include <stdio.h>
#include <stdlib.h>

#define MAX 128 //N/A

char my_string[MAX]; //Uninitialized Data (.bss)
char class_name[] = "System Programming"; //Initialized data (.data)
int global_integer = 3; //Initialized data (.data)

void print_global() { //Program code (text)
  printf("%d\n", global_integer); //Program code (text)
}

int square(int x) { //Program code (text)
  int result = x * x; //Local variable (stack)

  return result;
}

int main (int argc, char *argv[]) { //Program code (text)
  char *dynamic_memory; //Pointer variable (stack)
  dynamic_memory = malloc(MAX); //Allocated memory (heap)
  free(dynamic_memory); //Memory deallocated (heap)
  dynamic_memory = NULL; // Pointer variable set to null (stack)

  return 0;
}
```

---

### 6. Explain the difference between `malloc`, `calloc`, `realloc`

The three functions, `malloc`, `calloc`, and `realloc`, all have to do with memory in different ways. `malloc` allocates memory a size given in bytes, it will also not initialize allocated memory. `calloc`, however, allocates memory for an array of a certain number of items, with a certain size in bytes, and will initialize the allocated memory (unlike `malloc`). Then, much different from the other two, `realloc` will "reallocate" a previously allocated part of memory, so long as that memory was allocated by one of the functions in the malloc package.

---

### 7. Explain the difference between `brk` and `sbrk`. Why is `malloc` a better option of `brk`, and `sbrk`?

The system call `brk` will set the program break to a specific point, effectively changing the size of allocated memory for the proccess's data segment. Then, `sbrk` will adjust a given program break by a given `increment` amount. The reason that `malloc` is better then these two system calls is that it's standardized in the C language, easier to use in threaded programs and is thread-safe, and has a simplified interface to allocate memory in small units (bytes). `malloc` is essentially a more modern and built memory allocation feature, that used `brk` and `sbrk` as a foundation.

---

### 8. Explain why a shadow password file is needed

A shadow password file is often where user's encrypted passwords are stored. Previously UNIX systems stored passwords in /etc/passwd, which presented many security problems, as unprivileged system utilies had access to read them. The shadow password file, however, is only readable by privileged users, making it much more secure. The intention was to have all nonsensitive data in publically readable files, while the encrypted data is in the shadow password file, where its only readable by privileged programs.

---

### 9. What is the numeric equivalent of the `README.md` file’s permission bits below, and what is the command to give the file full access, across the board?

```sh
-rw-r--r-- 1  johnnythunders  staff  3476 May  1 11:57 README.md
```

To read permission bits numerically, when looking at it in groups of three (the owner, group, and then other sections), it goes in order of 4 2 1. If we look at the one above, we have 6 (rw- or 4 2 -), 4 (r-- or 4 - -), 4 (r-- or 4 - -).

Together, this makes the numerical equivalent `644`.

The way to give the file full access across the board is using the `chmod` function, which allows you to change that numerical equivalent however you like. For this case, the command to give it full access would be `chmod 777 README.md`.

---

### 10. What does `kill -9 1` do? Explain in detail

`kill -9` will force kill a process by using its process ID. This is because -9 is the signal number 9, which is SIGKILL, which will kill a process. The PID 1 in linux is the `init` system process, which is what is started by the kernel when booting up your system.

Now, what will it `actually` do is a slightly different answer. Your system will probably reject the request as part of a kernel safeguard (this is what happens when I attempt running this command in my ubuntu virutal machine: `Operation not permitted`).

But if by some miracle it does pass, your full system will crash. PID 1 is the parent of all processes, so killing it will leave no process for the system. Killing a parent process is usually not a terrible issue, as it will default that orphaned process to make PID 1 it's parent. But, if PID 1 is what is deleted, the entire system will crash.