#include "explorer.h"

#define MAX_NAME_LENGTH 128
#define MAX_BUFFER_LENGTH 1024
#define MAX_ARGS 16

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
    
    ssize_t ret = read(fd, buffer, MAX_BUFFER_LENGTH);

    off_t cur = lseek(fd, 0, SEEK_CUR);
    if (cur != (off_t) - 1){
        lseek(fd, 0, SEEK_SET);
    }
    
    if (ret == 0){
        printf("syscall: read(%d, buffer, buffer_length)\n", fd);
        printf("return: %ld\n", ret);
        printf("errno: %d\n", errno);
        printf("buffer: <EOF>\n"); 

        free(buffer);
        fflush(stdout);
        return;
    }
    
    buffer[ret] = '\0';

    printf("syscall: read(%d, buffer, buffer_length)\n", fd);
    printf("return: %ld\n", ret);
    printf("errno: %d\n", errno);
    printf("buffer:\n%s", buffer);

    free(buffer);
    fflush(stdout);
}

void syscall_write(int fd, char* buffer){
    errno = 0;

    int ret = write(fd, buffer, strlen(buffer));

    printf("syscall: write(%d, %s, %lu)\n", fd, buffer, strlen(buffer));
    printf("return: %d\n", ret);
    printf("errno: %d\n", errno);

    fflush(stdout);
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
            char* name = malloc(sizeof(char) * MAX_NAME_LENGTH);
            printf("Enter the name of the file >> ");
            scanf("%s", name);
            printf("\n");

            syscall_create(name);

        } else if (choice == 2){
            char* name = malloc(sizeof(char) * MAX_NAME_LENGTH);
            printf("Enter the name of the file >> ");
            scanf("%s", name);
            printf("\n");

            syscall_open(name);
        } else if (choice == 3){
            int fd;
            printf("Enter the file descriptor of the file >> ");
            scanf("%d", &fd);
            printf("\n");

            syscall_close(fd); 
        } else if (choice == 4){
            int fd;
            printf("Enter the file descriptor of the file >> ");
            scanf("%d", &fd);
            printf("\n");

            syscall_read(fd);        
        } else if (choice == 5){
            int fd;
            char buffer[MAX_BUFFER_LENGTH];
            printf("Enter the file descriptor of the file >> ");
            scanf("%d", &fd);
            printf("\n");
            printf("Enter what you want to write to the file >> ");
            fflush(stdin);
            
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}

            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            printf("\n");

            syscall_write(fd, buffer);
        } else if (choice == 6){
            syscall_fork();
        } else if (choice == 7){
            char** args = malloc(sizeof(char*) * MAX_ARGS);
            char* command = malloc(sizeof(char) * MAX_NAME_LENGTH);
            int counter = 1;

            printf("Enter the command you want to run >> ");
            scanf("%s", command);
            printf("\n"); 
            args[0] = command;
            
            char* arg = malloc(sizeof(char) * MAX_NAME_LENGTH);
            while (1){
                printf("Enter the arguments for the command (enter NULL to break from the loop) >> ");
                scanf("%s", arg);
                printf("\n");
                
                if (strcmp(arg, "NULL") == 0){
                    args[counter] = NULL;
                    counter++;
                    break;
                } else {
                    args[counter] = arg;
                    counter++;
                }
            }

            syscall_exec(args);
        } else if (choice == -1){
            running = 0;
        } else {
            printf("Choice (%d) not recognized\n", choice);
            return -1;
        }
    }
    

    return 0;
}
