#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    char* cmd = "echo";

    if(2 == argc) {
        if(strlen(argv[1]) > 4) {
            errx(1, "invalid command");
        }

        cmd = argv[1];
    }
    else if(1 != argc){
        errx(2, "invalid number of args");
    }

    char arg1[5], arg2[5];
    int l1 = 0, l2 = 0, rs;
    while((rs = read(0, arg1 + l1, 1)) == 1) {
        if(arg1[l1] == 0x20 || arg1[l1] == 0x0a) {
            while((rs = read(0, arg2 + l2, 1)) == 1) {
                if(arg2[l2] == 0x20 || arg2[l2] == 0x0a) {
                    break;
                }

                l2++;
                if(l2 > 4) {
                    errx(3, "invalid arg length");
                }
            }

            if(rs == -1) {
                errx(4, "reading failed");
            }
        }
        else {
            l1++;

            if(l1 > 4) {
                errx(3, "invalid arg length");
            }

            continue;
        }

        arg1[l1] = '\0';
        arg2[l2] = '\0';

        if(fork() == 0){
            if(execlp(cmd, cmd, arg1, (l2 > 0 ? arg2 : NULL), (char*)NULL) == -1) {
                exit(-1);
            }
        }
        
        int status;
        wait(&status);

        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            errx(5, "problematic child");
        }
        
        l1=0;
        l2=0;
    }

    if(rs == -1) {
        errx(4, "reading failed");
    }

    if(l1 > 0) {
        if(fork() == 0){
            if(execlp(cmd, cmd, arg1, (char*)NULL) == -1) {
                exit(-1);
            }
        }
        
        int status;
        wait(&status);

        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            errx(5, "problematic child");
        }
    }

    exit(0);
}
