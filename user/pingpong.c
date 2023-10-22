//
// Created by vlad on 21.09.23.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    int pid;
    int pipe1[2];
    int pipe2[2];
    char buff[64];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
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
        close(pipe1[1]);
        close(pipe2[0]);
        read(pipe1[0], buff, sizeof(buff));
        close(pipe1[0]);
        printf("%p: got \"%s\"\n", getpid(), buff);
        if (strlen(buff) > 0) {
            write(pipe2[1], "pong", 4);
        }
        close(pipe2[1]);
    } else {
        // parent
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], "ping", 4);
        close(pipe1[1]);
        read(pipe2[0], buff, sizeof(buff));
        close(pipe2[0]);
        printf("%p: got \"%s\"\n", getpid(), buff);
    }
    exit(0);
}

