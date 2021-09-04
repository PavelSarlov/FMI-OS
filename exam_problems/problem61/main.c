#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {

    if(argc != 2) {
        errx(1, "invalid number of args");
    }
     
    for(int i=0; i<3; i++) {
        int pp[2];
        if(pipe(pp) == -1) {
            errx(2, "plumbing failed");
        }

        if(fork() == 0) {
            close(pp[0]);
            dup2(pp[1], 1);
            close(pp[1]);

            if(i == 0) {
                if(execlp("find", "find", "-L", argv[1], "-printf", "%Ts %p\n", (char*)NULL) == -1) {
                    exit(-1);
                }
            }
            else if(i == 1) {
                if(execlp("sort", "sort", "-n", (char*)NULL) == -1) {
                    exit(-1);
                }
            }
            else if(i == 2) {
                if(execlp("tail", "tail", "-n", "1", (char*)NULL) == -1) {
                    exit(-1);
                }
            }

            exit(0);
        }

        close(pp[1]);
        dup2(pp[0], 0);
        close(pp[0]);

        int status;
        wait(&status);

        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            errx(3, "problematic child");
        }

        if(i == 2) {
            if(execlp("cut", "cut", "-d", " ", "-f", "2", (char*)NULL) == -1) {
                errx(4, "exec failed");
            }
        }
    }

    exit(0);
}
