//
// Created by vlad on 21.09.23.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    pde_t pid;
    int pipefd[2];
    char buff[128];

    if (pipe(pipefd) == -1) {
        fprintf(2, "Unable to create pipe!");
        exit(1);
    }
    pid = fork();

    if (pid == -1) {
        fprintf(2, "Unable to create subprocess!");
        exit(1);
    }

    if (pid == 0) {
        // child
        read(pipefd[0], buff, sizeof(buff));
        printf("%p: got %s\n", getpid(), buff);
        if (strlen(buff) > 0) {
            write(pipefd[1], "ping", 4);
        }
    } else {
        // parent
        printf("child pid: %p\n", pid);
        write(pipefd[1], "pong", 4);
        wait(0);
        read(pipefd[0], buff, sizeof(buff));
        printf("%p: got %s\n", getpid(), buff);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    exit(0);
}

