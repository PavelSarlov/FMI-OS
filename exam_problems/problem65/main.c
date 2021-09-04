#include <sys/wait.h>
#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    if(argc != 2) {
        errx(1, "invalid num of args");
    }

    int fd = open(argv[1], O_RDONLY);

    if(-1 == fd) {
        errx(2, "opening file failed");
    }
   
    int arr[8];
    int iterator = 0;
    char data[16];
    int rs;
    uint16_t end_result = 0;
    while((rs = read(fd, data, sizeof(data))) == sizeof(data)) {
 
        int pf[2];
        if(-1 == pipe(pf)) {
            errx(5, "pipe failed");
        }

        int pid = fork();

        if(0 == pid) {

            close(pf[0]);

            char* filename = data;
            uint32_t offset = *((uint32_t*)(data + 8));
            uint32_t length = *((uint32_t*)(data + 12));

            int fd1 = open(filename, O_RDONLY);

            if(-1 == fd1) {
                close(pf[1]);
                errx(2, "opening file failed");
            }

            if(lseek(fd1, offset * sizeof(uint16_t), SEEK_SET) == -1) {
                close(fd1);
                close(pf[1]);
                errx(7, "lseek failed");
            }
            uint16_t xor_result = 0;

            for(uint32_t i=0; i<length; i++) {
                uint16_t element;
                if(read(fd1, &element, sizeof(element)) == -1) {
                    close(fd1);
                    close(pf[1]);
                    errx(3, "reading failed");
                }

                xor_result ^= element;
            }

            close(fd1);
            
            if(write(pf[1], &xor_result, sizeof(xor_result)) != sizeof(xor_result)) {
                close(pf[1]);
                errx(5, "writing to pipe failed");
            }

            printf("%d\n", xor_result);

            close(pf[1]);
            exit(0);
        }

        arr[iterator++] = pf[0];
        close(pf[1]);
    }
    
    wait(NULL);

    for(int i = 0; i < iterator && arr[i] != -1; i++) {
        uint16_t child_result;
        if(read(arr[i], &child_result, sizeof(child_result)) != sizeof(child_result)) {
            errx(6, "reading from pipe failed");
        }

        end_result ^= child_result;

        close(arr[i]);
    }

    close(fd);

    if(-1 == rs) {
        errx(3, "reading failed");
    }
    if (0 != rs && sizeof(data) != rs) {
        errx(4, "wrong file format");
    }

    printf("result: %dB\n", end_result);

    exit(0);
}
