#pragma once
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void greeting();
void syscall_create();
void syscall_open();
void syscall_close();
void syscall_read();
void syscall_write();
void syscall_fork();

