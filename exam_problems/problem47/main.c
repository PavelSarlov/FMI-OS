#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
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

    if(argc != 4) {
        errx(1, "invalid number of args");
    }

    int fd1 = open(argv[1], O_RDONLY),
        fd2 = open(argv[2], O_RDONLY),
        fd3 = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644);

    if(fd1 == -1 || fd2 == -1 || fd3 == -1) {
        cfd(3,fd1,fd2,fd3);
        errx(2, "opening fail failed");
    }

    uint8_t buf[4096];
    int rs = 0;
    while((rs = read(fd2, &buf, sizeof(buf))) > 0) {
        if(write(fd3, &buf, rs) != rs) {
            cfd(3,fd1,fd2,fd3);
            errx(4, "writing failed");
        }
    }

    if(rs == -1) {
        cfd(3,fd1,fd2,fd3);
        errx(3, "reading failed");
    }

    int size = lseek(fd2,0,SEEK_END);
    if(size==-1) {
        cfd(3,fd1,fd2,fd3);
        errx(5, "lseek failed");
    }

    uint16_t offset;
    uint8_t b1,b2;
    while((rs = read(fd1,&offset,sizeof(offset))) == sizeof(offset) &&
            (rs = read(fd1,&b1,sizeof(b1))) == sizeof(b1) &&
            (rs = read(fd1,&b2,sizeof(b2))) == sizeof(b2)) {
        if(offset>size) {
            cfd(3,fd1,fd2,fd3);
            errx(6, "offset too high");
        }

        if(lseek(fd2,offset,SEEK_SET) == -1 ||
                lseek(fd3,offset,SEEK_SET) == -1) {
            cfd(3,fd1,fd2,fd3);
            errx(5, "lseek failed");
        }

        uint8_t c;
        if(read(fd2,&c,sizeof(c)) != sizeof(c)) {
            cfd(3,fd1,fd2,fd3);
            errx(3, "reading failed");
        }

        if(c!=b1) { 
            cfd(3,fd1,fd2,fd3);
            errx(7, "different byte");
        }

        if(write(fd3,&b2,sizeof(b2)) != sizeof(b2)) {
            cfd(3, fd1,fd2,fd3);
            errx(4, "writing failed");
        }
    }

    if(rs == -1) { 
        cfd(3,fd1,fd2,fd3);
        errx(3, "reading failed");
    }

    exit(0);
}
