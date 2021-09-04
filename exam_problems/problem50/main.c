#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <stdarg.h>

void cfd(int num, ...) {
    va_list fds;
    va_start(fds,num);

    for(int i=0; i<num; i++) {
        int fd = va_arg(fds, int);
        if(fd != -1) {
            close(fd);
        }
    }

    va_end(fds);
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        errx(1, "invalid num of args");
    }

    int fd1 = open(argv[1], O_RDONLY),
        fd2 = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0644);

    if(fd1 == -1 || fd2 == -1) {
        cfd(2, fd1, fd2);
        errx(2, "opening fail failed");
    }

    int count = lseek(fd1, 0, SEEK_END);
    if(count == -1 || lseek(fd1, 0, SEEK_SET) == -1) {
        cfd(2,fd1,fd2);
        errx(3, "lseek failed");
    }

    uint32_t buf[4096];
    int rs;
    while((rs = read(fd1,&buf,sizeof(buf))) > 0) {
        if(write(fd2,&buf,rs) != rs) {
            cfd(2,fd1,fd2);
            errx(5, "writing failed");
        }
    }

    if(rs == -1) {
        cfd(2,fd1,fd2);
        errx(4, "reading failed");
    }

    for(int i=0; i<count; i+=sizeof(uint32_t)) {
        if(lseek(fd2, i, SEEK_SET) == -1) {
            cfd(2,fd1,fd2);
            errx(3, "lseek failed");
        }

        uint32_t min;
        if(read(fd2,&min,sizeof(min)) != sizeof(min)) {
            cfd(2,fd1,fd2);
            errx(4, "reading failed");
        }

        uint32_t start = min;
        uint32_t num;
        int offset;
        int j = i + sizeof(num);
        while((rs = read(fd2,&num,sizeof(num))) == sizeof(num)) {
            if(num < min) {
                min = num;
                offset = j;
            }
            j+=sizeof(num);
        }

        if(rs == -1) {
            cfd(2,fd1,fd2);
            errx(4, "reading failed");
        }

        if(lseek(fd2, offset, SEEK_SET) == -1 ||
                write(fd2,&start,sizeof(start)) != sizeof(start) ||
                lseek(fd2, i, SEEK_SET) == -1 ||
                write(fd2,&min,sizeof(min)) != sizeof(min)) {
            cfd(2,fd1,fd2);
            errx(5, "writing failed");
        }
    }

    cfd(2,fd1,fd2);
    exit(0);
}
