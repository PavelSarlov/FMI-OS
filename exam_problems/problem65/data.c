#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) { 
    
    char arr[8];
    uint16_t elem;
    uint32_t offset, length;

    int fd = open("test", O_RDWR | O_TRUNC | O_CREAT, 0644);

    for(int i=0; i<8; i++) {
        arr[i]=0x00;
    }

    //?
    sprintf(arr, "pesho");
    offset = 5;
    length = 3;
    write(fd, arr, 8);
    write(fd, &offset, 4);
    write(fd, &length, 4);
    
    for(int i=0; i<8; i++) {
        arr[i]=0x00;
    }

    sprintf(arr, "tri");
    offset = 4;
    length = 9;
    write(fd, arr, 8);
    write(fd, &offset, 4);
    write(fd, &length, 4);
    
    for(int i=0; i<8; i++) {
        arr[i]=0x00;
    }

    sprintf(arr, "foobar");
    offset = 15;
    length = 13;
    write(fd, arr, 8);
    write(fd, &offset, 4);
    write(fd, &length, 4);

    exit(0);
}
