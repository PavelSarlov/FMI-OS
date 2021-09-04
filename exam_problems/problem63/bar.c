#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    if(argc != 2) {
        errx(1, "invalid num of args");
    }

    int fd = open("fifo", O_RDONLY);
    if(fd == -1) {
        errx(2, "failed to open fifo");
    }

    dup2(fd, 0);
    close(fd);

    if(execl(argv[1], argv[1], (char*)NULL) == -1) {
        errx(3, "exec failed");
    }

    exit(0);
}
