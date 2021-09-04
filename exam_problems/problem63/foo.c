#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    if(argc != 2) {
        errx(1, "invalid number of args");
    }

    mkfifo("fifo", 0644);

    int fd = open("fifo", O_WRONLY);

    if(fd == -1) {
        errx(3, "open failed");
    }

    dup2(fd, 1);
    close(fd);

    if(execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
        errx(4, "exec failed");   
    }

    exit(0);
}
