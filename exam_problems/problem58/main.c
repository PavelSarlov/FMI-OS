#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    
    while(printf("your command: ") != -1) {
        char path[30] = "/bin/";
        char cmd[20];
        scanf("%s", cmd);
        strcat(path, cmd);

        if(fork() == 0) {
            if(execl(path, cmd, (char*)NULL) == -1) {
                errx(2, "execl failed");
            }
        }
        wait(NULL);
    }

    exit(0);
}
