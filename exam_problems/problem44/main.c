#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void errmsg(int code) {
    switch(code) {
        case 1:
            errx(code, "Invalid number of args");
        case 2:
            errx(code, "Reading failed");
        case 3:
            errx(code, "Writing failed");
        case 4:
            errx(code, "'lseek' failed");
        default:
            exit(0);
    }
}

int main(int argc, char* argv[]) {
    
    if(argc != 5) {
        errx(1, "invalid number of args");
    }

    int fd1,fd2,fd3,fd4;
    int errcode = 0;

    if((fd1 = open(argv[1], O_RDONLY)) == -1 ||
            (fd2 = open(argv[2], O_RDONLY)) == -1 ||
            (fd3 = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1 ||
            (fd4 = open(argv[4], O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1) {
        err(errno, NULL);
    }

    int read_status = 0;
    uint16_t offset_in, offset_out = 0;
    while((read_status = read(fd2, &offset_in, sizeof(offset_in))) == sizeof(offset_in)) {

        uint8_t length[2];

        if(read(fd2, &length, sizeof(length)) != sizeof(length)) {
            printf("1\n");
            errcode = 2;
            goto finish;
        }

        if(lseek(fd1, offset_in, SEEK_SET) == -1) {
            errcode = 4;
            goto finish;
        }

        char* str = malloc(length[0]+1);
        str[length[0]]='\0';

        if(read(fd1, str, length[0]) != length[0]) {
            errcode = 2;
            free(str);
            printf("2\n");
            goto finish;
        }

        if(str[0] < 0x41 || str[0] > 0x5A) {
            free(str);
            continue;
        }

        if(write(fd3, str, length[0]) != length[0] ||
                write(fd4, &offset_out, sizeof(offset_out)) != sizeof(offset_out) ||
                write(fd4, &length, sizeof(length)) != sizeof(length)) {
            free(str);
            errcode = 3;
            goto finish;
        }

        offset_out+=length[0];

        free(str);
    }

    if(read_status != sizeof(offset_in) && read_status != 0) {
        errcode = 2;
        printf("3\n");
        goto finish;
    }

finish:
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    errmsg(errcode);
}
