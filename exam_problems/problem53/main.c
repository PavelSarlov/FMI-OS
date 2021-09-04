#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    
    if(2 != argc) {
        errx(1, "invalid num of args");
    }

    int fd = open(argv[1], O_RDONLY);
    if(-1 == fd) {
        errx(2, "failed to open file '%s'", argv[1]);
    }

    uint32_t data[4], users[2048][2];
    int N = 0, size = 0, rs;
    uint64_t sum = 0;

    while((rs = read(fd, &data, sizeof(data))) == sizeof(data)) {
        uint32_t time = data[3] - data[2];
        N++;
        sum += time;

        char found = 0;
        for(int i=0; i<size; i++) {
            if(users[i][0] == data[0]) {
                if(users[i][1] < time) {
                    users[i][1] = time;
                }
                found = 1;
                break;
            }
        }
        if(!found) {
            size++;
            users[0][0] = data[0];
            users[0][1] = time;
        }
    }

    if(sizeof(data) != rs && 0 != rs) {
        close(fd);
        errx(3, "reading failed");
    }

    close(fd);

    double D = (double)sum/N;

    for(int i=0; i<size; i++){
        if((uint64_t)users[i][1]*users[i][1] > D) {
            printf("user: %u, session: %u", users[i][0], users[i][1]);
        }
    }

    exit(0);
}
