#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    if(argc != 2) {
        errx(1, "invalid number of args");
    }

    int pp[2];

    if(pipe(pp) == -1) {
        errx(2, "piping failed");
    }

    int pid = fork();

    if(pid == 0) {
        close(pp[0]);

        if(dup2(pp[1], 1) == -1) {
            close(pp[1]);
            errx(3, "dup2 failed");
        }

        if(execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
            errx(4, "cat failed");
        }
    }

    close(pp[1]);

    int status;
    if(wait(&status) == -1) {
        close(pp[0]);
        errx(5, "waiting failed");
    }
    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        close(pp[0]);
        errx(6, "child was murdered ;(");
    }

    if(dup2(pp[0], 0) == -1) {
        close(pp[0]);
        errx(3, "dup2 failed");
    }
    close(pp[0]);
    
    if(execlp("sort", "sort",  (char*)NULL) == -1) {
        errx(7, "sort failed");
    }

    exit(0);
}
