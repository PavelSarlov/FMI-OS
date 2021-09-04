#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = open("f1.idx", O_WRONLY|O_CREAT, 0644);

    if(fd == -1) {
        err(errno, NULL);
    }

    uint16_t a = 0x0000, b = 0x014f, c = 0x014e;
    uint8_t aa = 0x05, bb = 0x02, cc = 0x03;
    uint8_t blank = 0x00;
    write(fd, &a, sizeof(a));
    write(fd, &aa, sizeof(aa));
    write(fd, &blank, sizeof(blank));
    write(fd, &b, sizeof(a));
    write(fd, &bb, sizeof(aa));
    write(fd, &blank, sizeof(blank));
    write(fd, &c, sizeof(a));
    write(fd, &cc, sizeof(aa));
    write(fd, &blank, sizeof(blank));
    
    close(fd);
    exit(0);
}
