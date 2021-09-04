#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdint.h>

int main(int argc, char* argv[]) {
    
    if(argc!=3) {
        errx(1, "invalid num of args");
    }

    int fd1, fd2;

    if((fd1 = open(argv[1], O_RDONLY)) == -1) {
        errx(2, "failed to open file '%s'", argv[1]);
    }
    if((fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1) {
        close(fd1);
        errx(2, "failed to open file '%s'", argv[2]);
    }

    uint16_t arr[UINT16_MAX + 1];
    for(int i=0; i<=UINT16_MAX; i++) {
        arr[i]=0;
    }
    
    uint16_t num;
    int rs;
    while((rs = read(fd1, &num, sizeof(num))) == sizeof(num)) {
        arr[num]++;
    }

    if(sizeof(num) != rs && 0 != rs) {
        close(fd1);
        close(fd2);
        errx(3, "reading failed");
    }

    for(int i=0; i<=UINT16_MAX; i++) {
        while(arr[i]--) {
            if(write(fd2, &i, sizeof(uint16_t)) != sizeof(uint16_t)) {
                close(fd1);
                close(fd2);
                errx(4, "writing failed");
            }
        }
    }

    close(fd1);
    close(fd2);

    exit(0);
}
