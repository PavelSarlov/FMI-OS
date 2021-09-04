#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));

    int fd = open("numbers", O_RDWR|O_CREAT, 0644);

    if(fd == -1) {
        err(errno, NULL);
    }

    if(argc>1) {
        for(int i=0; i<10; i++) {
            uint32_t num = rand();

            if(write(fd, &num, sizeof(num)) != sizeof(num)) {
                close(fd);
                errx(1, "Failed writing");
            }
        }
    }
    else {
        uint32_t n;
        while(read(fd, &n, sizeof(n)) == sizeof(n)) {
            printf("%d\n", n);
        }

        if(errno != 0) {
            close(fd);
            errx(2, "Failed writing");
        }
    }

    close(fd);

    exit(0);
}
