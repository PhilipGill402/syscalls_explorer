#pragma once
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

void greeting();
void syscall_create(char* name);
void syscall_open(char* name);
void syscall_close(int fd);
void syscall_read(int fd);
void syscall_write(int fd, char* buffer);
void syscall_fork();
void syscall_exec(char* args[]);
