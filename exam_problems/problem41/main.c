#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {

    if(argc != 2) {
        errx(1, "Invalid number of arguments");
    }

    int fd = open(argv[1], O_RDWR);

    if(fd == -1) {
        err(errno, NULL);
    }

    unsigned int occurences[256];
    for(int i = 0; i <= 256; i++) {
        occurences[i] = 0;
    }

    unsigned char b;
    int read_status;
    while((read_status = read(fd, &b, sizeof(b))) > 0) {
        occurences[b]++;
    }

    if (read_status == -1) {
        close(fd);
        errx(2, "Read failed");
    }

    lseek(fd, 0, SEEK_SET);

    for(int i = 0; i < 256; i++) {
        b = i;
        while(occurences[i]-- > 0) {
            if(write(fd, &b, 1) == -1) {
                close(fd);
                errx(3, "Write failed");
            }
        }
    }

    exit(0);
}
