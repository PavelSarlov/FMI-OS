#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

void cfds(int count, ...) {
    va_list fds;
    va_start(fds, count);

    for(int i=0; i<count; i++) {
        close(va_arg(fds, int));
    }

    va_end(fds);
}

int main(int argc, char* argv[]) {
    
    if(4 != argc) {
        errx(1, "invalid num of args");
    }

    int fd1 = open(argv[1], O_RDONLY),
        fd2 = open(argv[2], O_RDONLY),
        fd3 = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644);

    if(-1 == fd1) {
        errx(2, "failed to open file '%s'", argv[1]);
    }
    if(-1 == fd2) {
        cfds(1,fd1);
        errx(2, "failed to open file '%s'", argv[2]);
    }
    if(-1 == fd3) {
        cfds(2,fd1,fd2);
        errx(2, "failed to open file '%s'", argv[3]);
    }
   
    char c;
    while(read(fd2, &c, sizeof(c)) > 0) {
        if(write(fd3, &c, sizeof(c)) == -1) {
            cfds(3,fd1,fd2,fd3);
            errx(4, "writing failed");
        }
    }
   
    uint8_t header[16];
    int rs;

    while((rs = read(fd1, &header, sizeof(header)) == sizeof(header))) {
        uint32_t magic = *((uint32_t*)header);
        uint8_t hVersion = *((uint8_t*)(header + sizeof(magic)));
        uint8_t dVersion = *((uint8_t*)(header + sizeof(magic) + sizeof(hVersion)));
        uint16_t count = *((uint16_t*)(header + sizeof(magic) + sizeof(hVersion) + sizeof(dVersion)));

        if(0x00 == dVersion) {
            uint16_t offset;
            uint8_t old, new;

            if(read(fd1, &offset, sizeof(offset)) != sizeof(offset) ||
                    read(fd1, &old, sizeof(old)) != sizeof(old) ||
                    read(fd1, &new, sizeof(new)) != sizeof(new)) {
                cfds(3,fd1,fd2,fd3);
                errx(3, "reading failed");
            }

            uint8_t byte;
            if(count < offset ||
                    lseek(fd2, offset * sizeof(old), SEEK_SET) == -1 ||
                    read(fd2, &byte, sizeof(byte)) != sizeof(byte) ||
                    byte != old) {
                cfds(3,fd1,fd2,fd3);
                errx(6, "invalid offset OR lseek failed OR reading failed OR invalid byte");
            }

            if(lseek(fd3, offset * sizeof(new), SEEK_SET) == -1 ||
                    write(fd3, &new, sizeof(new)) != sizeof(new)) {
                cfds(3,fd1,fd2,fd3);
                errx(7, "lseek failed OR writing failed");
            }
        }
        else if (0x01 == dVersion) {
            uint32_t offset;
            uint16_t old, new;

            if(read(fd1, &offset, sizeof(offset)) != sizeof(offset) ||
                    read(fd1, &old, sizeof(old)) != sizeof(old) ||
                    read(fd1, &new, sizeof(new)) != sizeof(new)) {
                cfds(3,fd1,fd2,fd3);
                errx(3, "reading failed");
            }

            uint16_t word;
            if(count < offset ||
                    lseek(fd2, offset * sizeof(old), SEEK_SET) == -1 ||
                    read(fd2, &word, sizeof(word)) != sizeof(word) ||
                    word != old) {
                cfds(3,fd1,fd2,fd3);
                errx(6, "invalid offset OR lseek failed OR reading failed OR invalid byte");
            }

            if(lseek(fd3, offset * sizeof(new), SEEK_SET) == -1 ||
                    write(fd3, &new, sizeof(new)) != sizeof(new)) {
                cfds(3,fd1,fd2,fd3);
                errx(7, "lseek failed OR writing failed");
            }
        }
        else {
            cfds(3,fd1,fd2,fd3);
            errx(8, "invalid data version");
        }
    }

    if(rs != sizeof(header) && rs != 0) {
        cfds(3,fd1,fd2,fd3);
        errx(3, "reading failed");
    }

    cfds(3,fd1,fd2,fd3);
    exit(0);
}
