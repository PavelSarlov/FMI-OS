#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

int main() {

    int fd1, fd2, fd3;

    if((fd1 = open("f1", O_RDONLY)) == -1) {
        err(errno, NULL);
    }
    if((fd2 = open("f2", O_RDONLY)) == -1) {
        err(errno, NULL);
    }
    if((fd3 = open("f3", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) { 
        err(errno, NULL);
    }

    uint32_t interval[2];
    int read_status;
    while((read_status = read(fd1, &interval, sizeof(interval))) == sizeof(interval)) {
        uint32_t start = interval[0];
        uint32_t size = interval[1];

        if(-1 == lseek(fd2, start * sizeof(uint32_t), SEEK_SET)) {
            close(fd1);
            close(fd2);
            close(fd3);
            errx(2, "lseek failed");
        }

        for(int i = 0; i < (int)size; i++) {
            uint32_t element;

            if(read(fd2, &element, sizeof(element)) != sizeof(element)) {
                close(fd1);
                close(fd2);
                close(fd3);
                errx(3, "read fd2 failed");
            }

            if(write(fd3, &element, sizeof(element)) != sizeof(element)) {
                close(fd1);
                close(fd2);
                close(fd3);
                errx(4, "write failed");
            }
        }
    }

    close(fd1);
    close(fd2);
    close(fd3);

    if(read_status != 0 && read_status != 8) {
        errx(1, "read failed");
    }

    exit(0);
}
