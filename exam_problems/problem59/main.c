#include <sys/types.h>
#include <err.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    
    int pp[2];
    if(pipe(pp) == -1) {
        errx(1, "piping failed");
    }

    if(fork() == 0) {
        close(pp[0]);
        dup2(pp[1], 1);
        close(pp[1]);
        
        if(execlp("cut", "cut", "-d", ":", "-f", "7", "/etc/passwd", (char*)NULL) == -1) {
            errx(2, "cut failed");
        }
    }
    wait(NULL);
    
    close(pp[1]);
    dup2(pp[0], 0);
    close(pp[0]);
    
    if(pipe(pp) == -1) {
        errx(1, "piping failed");
    }

    if(fork() == 0) {
        close(pp[0]);
        dup2(pp[1], 1);
        close(pp[1]);

        if(execlp("sort", "sort", (char*)NULL) == -1) {
            errx(3, "sort failed");
        }
    }
    wait(NULL);
    
    close(pp[1]);
    dup2(pp[0], 0);
    close(pp[0]);
    
    if(pipe(pp) == -1) {
        errx(1, "piping failed");
    }

    if(fork() == 0) {
        close(pp[0]);
        dup2(pp[1], 1);
        close(pp[1]);

        if(execlp("uniq", "uniq", "-c", (char*)NULL) == -1) {
            errx(4, "uniq failed");
        }
    }
    wait(NULL);

    close(pp[1]);
    dup2(pp[0], 0);
    close(pp[0]);
    
    if(execlp("sort", "sort", "-n", (char*)NULL) == -1) {
        errx(3, "sort failed");
    }

    exit(0);
}
