#include "explorer.h"

#define MAX_NAME_LENGTH 128
#define MAX_BUFFER_LENGTH 1024

void greeting(){
    printf("1. create file with creat()\n");
    printf("2. open file with open()\n");
    printf("3. close file with close()\n"); 
    printf("4. read from file with read()\n");
    printf("5. write to file with write()\n");
    printf("6. fork process with fork()\n");
    printf("7. execute new program with execvp()\n");
}

void syscall_create(char* name){
    errno = 0;
    int fd = creat(name, S_IRWXU);

    printf("syscall: creat(%s, S_IRWXU)\n", name);
    printf("return: %d\n", fd);
    printf("errno: %d\n", errno);
    
    free(name);
}

void syscall_open(char* name){
    errno = 0;
    int fd = open(name, O_RDWR);

    printf("syscall: open(%s, O_RDWR)\n", name);
    printf("return: %d\n", fd);
    printf("errno: %d\n", errno);
    
    free(name);
}

void syscall_close(int fd){
    int ret = close(fd);
    errno = 0;
    
    printf("syscall: close(%d)\n", fd);
    printf("return: %d\n", ret);
    printf("errno: %d\n", errno);
}

void syscall_read(int fd){
    char* buffer = malloc(sizeof(char) * MAX_BUFFER_LENGTH);
    errno = 0;
    
    int ret = read(fd, buffer, MAX_BUFFER_LENGTH);

    printf("syscall: read(%d, buffer, buffer_length)\n", fd);
    printf("return: %d\n", ret);
    printf("errno: %d\n", errno);
    printf("buffer:\n%s", buffer);
}

void syscall_write(int fd, char* buffer){
    errno = 0;

    int ret = write(fd, buffer, strlen(buffer));

    printf("syscall: write(%d, %s, %lu)\n", fd, buffer, strlen(buffer));
    printf("return: %d\n", ret);
    printf("errno: %d\n", errno);
}

void syscall_fork(){
    pid_t pid = fork();
    int status;
    
    errno = 0;

    //child
    if (pid == 0){
        printf("syscall: fork()\n");
        printf("return: %d\n", pid);
        printf("errno: %d\n", errno);
    
        fflush(stdout);
        _exit(0);
    }  else if (pid < 0){
        printf("syscall: fork()\n");
        printf("return: %d\n", pid);
        printf("errno: %d\n", errno);

        return;
    }
    
    //parent
    waitpid(pid, &status, 0);

    printf("syscall: fork()\n");
    printf("return: %d\n", pid);
    printf("errno: %d\n", errno);
    printf("exit status: %d\n", status);

    fflush(stdout);
}

void syscall_exec(char* args[]){
    pid_t pid = fork();
    int status;
    int ret = 0;
    char* str = args[1];
    errno = 0;

    //child
    if (pid == 0){
        ret = execvp(args[0], args); 
        printf("syscall: execvp(\"%s\"", args[0]);

        while (str != NULL){
            printf("%s, ", str);
            str++; 
        }
        printf(")\n");
        printf("return: %d\n", ret);
        printf("errno: %d\n", errno);

        _exit(0);

    }  else if (pid < 0){
        printf("syscall: execvp()\n");
        printf("return: %d\n", pid);
        printf("errno: %d\n", errno);

        return;
    }
    
    //parent
    waitpid(pid, &status, 0);

    printf("syscall: execvp(\"%s\"", args[0]);

    while (str != NULL){
        printf("%s, ", str);
        str++; 
    }
    printf(")\n");   
    printf("return: %d\n", ret);
    printf("errno: %d\n", errno);
    fflush(stdout);
}

int main(){
    setvbuf(stdout, NULL, _IOLBF, 0);
    int choice; 
    int running = 1;
    greeting(); 
    
    while (running){
        printf("\n>> ");
        scanf("%d", &choice);

        if (choice == 1){
            syscall_create();
        } else if (choice == 2){
            syscall_open();
        } else if (choice == 3){
            syscall_close(); 
        } else if (choice == 4){
            syscall_read();        
        } else if (choice == 5){
            syscall_write();
        } else if (choice == 6){
            syscall_fork();
        } else if (choice == 7){
            syscall_exec();
        } else if (choice == -1){
            running = 0;
        } else {
            printf("Choice (%d) not recognized\n", choice);
            return -1;
        }
    }
    

    return 0;
}
