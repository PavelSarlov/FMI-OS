#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  

void cfd(int num, ...) {
    va_list fds;
    va_start(fds, num);

    for(int i=0; i<num; i++) {
        int fd = va_arg(fds,int);
        if(fd != -1) {
            close(fd);
        }
    }

    va_end(fds);
}

int main(int argc, char* argv[]) {
    
    if(argc!=4) {       
        errx(1, "invalid number of args");
    }

    int fd1=open(argv[1], O_RDONLY),
        fd2=open(argv[2], O_RDONLY),
        fd3=open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644);

    if(fd1 == -1 || fd2 == -1 || fd3 == -1) {
        cfd(3,fd1,fd2,fd3);
        errx(2, "failed opening file");
    }

    int s1=lseek(fd1, 0, SEEK_END),
        s2=lseek(fd2, 0, SEEK_END);

    if(s1 == -1 || s2 == -1) {
        cfd(3,fd1,fd2,fd3);
        errx(3, "lseek failed");
    }

    if(s1 != s2) {
        cfd(3,fd1,fd2,fd3);
        errx(4, "different sized files");
    }

    if(lseek(fd1,0,SEEK_SET) == -1 || lseek(fd2,0,SEEK_SET) == -1) {
        cfd(3,fd1,fd2,fd3);
        errx(3, "lseek failed");
    }

    uint8_t b1,b2;
    int r1,r2;
    uint16_t offset = 0;
    while((r1=read(fd1,&b1,sizeof(b1))) == sizeof(b1) && (r2=read(fd2,&b2,sizeof(b2))) == sizeof(b2)) {
        if(r1 == -1 || r2 == -1) {
            cfd(3,fd1,fd2,fd3);
            errx(5, "reading failed");
        }

        if(b1!=b2) {
            if(write(fd3,&offset,sizeof(offset)) != sizeof(offset) ||
                    write(fd3, &b1, sizeof(b1)) != sizeof(b1) ||
                    write(fd3, &b2, sizeof(b2)) != sizeof(b2)) {
                cfd(3,fd1,fd2,fd3);
                errx(6, "writing failed");
            }
        }

        offset++;
    }
    
    cfd(3,fd1,fd2,fd3);
    exit(0);
}
