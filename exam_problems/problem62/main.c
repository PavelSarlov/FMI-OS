#define _GNU_SOURCE

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char* argv[]) {
    
    if(argc < 3) {
        errx(1, "missing arguments");
    }

    int length;
    sscanf(argv[1], "%d", &length);

    char* path = malloc(strlen(argv[2]) + 3);
    sprintf(path, "./%s", argv[2]);

    int start_time;
    int fd = open("run.log", O_WRONLY|O_CREAT|O_TRUNC, 0644);

    if(fd == -1) {
        free(path);
        errx(2, "failed to open file");
    }

    while(1) {
        if(fork() == 0) {
            close(fd);
            start_time = time(NULL);
            if(execv(path, (argv + 2)) == -1) {
                exit(-1);
            }
        }

        int status;
        wait(&status);
        int end_time = time(NULL);
        int child_exit = WEXITSTATUS(status);

        dprintf(fd, "%d %d %d", start_time, end_time, child_exit);

        if(child_exit != 0 && end_time-start_time < length) {
            break;
        }
    }

    free(path);
    close(fd);
    exit(0);
}
