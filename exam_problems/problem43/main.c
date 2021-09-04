#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[]) {

    if(argc!=2) {
        errx(1, "Invalid number of arguments");
    }

    int fd = open(argv[1], O_RDWR);

    if(fd == -1) {
        err(errno, NULL);
    }
    
    int size = lseek(fd, 0, SEEK_END) / sizeof(uint32_t);
    if(size == -1 || lseek(fd,0,SEEK_SET) == -1) {
        close(fd);
        errx(2, "lseek failed");
    }

    int x = size, y=0;
    while(x--) {
        if(lseek(fd, y*sizeof(uint32_t), SEEK_SET) == -1) {
            close(fd);
            errx(2, "lseek failed");
        }

        uint32_t smallest = UINT32_MAX, current;
        int pos = 0;

        if(read(fd, &current, sizeof(current)) != sizeof(current)) {
            close(fd);
            errx(3, "reading failed");
        }

        for(int i=y; i<size; i++) {
            if(lseek(fd,i*sizeof(uint32_t),SEEK_SET) == -1) {
                close(fd);
                errx(2, "lseek failed");
            }

            uint32_t num;
            if(read(fd, &num, sizeof(num)) != sizeof(num)) {
                close(fd);
                errx(3, "reading failed");
            }

            if(num <= smallest) {
                smallest = num;
                pos = i;
            }
        }

        // swapping
        if(lseek(fd, pos*sizeof(uint32_t), SEEK_SET) == -1) {
            close(fd);
            errx(2, "lseek failed");
        }
        if(write(fd, &current, sizeof(current)) != sizeof(current)) {
            close(fd);
            errx(4, "writing failed");
        }
        if(lseek(fd, y*sizeof(uint32_t), SEEK_SET) == -1) {
            close(fd);
            errx(2, "lseek failed");
        }
        if(write(fd, &smallest, sizeof(smallest)) != sizeof(smallest)) {
            close(fd);
            errx(4, "writing failed");
        }

        y++;
    }

    close(fd);
    exit(0);
}
