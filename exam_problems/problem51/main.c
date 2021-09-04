#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>

enum OP { C,D };

int main(int argc, char* argv[]) {

    if(argc == 1) {
        errx(1, "missing parameters");
    }

    int operation, start, end;
    char delim;

    if(strcmp(argv[1], "-c") == 0) {
        if(argc < 3) {
            errx(5, "missing char position");
        } 

        operation = C;
        start = argv[2][0] - '0';
        end = start;

        if(strlen(argv[2]) == 3) {
            end = argv[2][2] - '0';
        }
        else if (strlen(argv[2]) != 1) {
            errx(9, "invalid digits");
        }

        if(start < 1 || start > 9 || end < 1 || end > 9 || start > end) {
            errx(3, "invalid char position/range");
        }
    }
    else if(strcmp(argv[1], "-d") == 0) {
        if (argc < 5) {
            errx(6, "missing parameters");
        }

        operation = D;
        delim = argv[2][0];

        if (strcmp(argv[3],"-f") != 0) {
            errx(7, "invalid parameter '%s'", argv[3]);
        }

        start = argv[4][0] - '0';
        end = start;
        
        if(strlen(argv[4]) == 3) {
            end = argv[4][2] - '0';
        }
        else if (strlen(argv[4]) != 1) {
            errx(9, "invalid digits");
        }

        if(start < 1 || start > 9 || end < 1 || end > 9 || start > end) {
            errx(3, "invalid field position/range");
        }
    }
    else {
        errx(4, "invalid parameters");
    }

    int line = 1, ccount=1, fcount=1, rs = 0;
    char c;
    while((rs=read(0, &c, sizeof(c))) > 0) {
        if(c == '\n') {
            if(write(1, &c, sizeof(c)) != sizeof(c)) {
                errx(8, "writing failed");
            }

            ccount = 1;
            fcount = 1;
            line++;
            continue;
        }

        if(operation == C) {
            if(ccount >= start && ccount <= end) {
                if(write(1, &c, sizeof(c)) != sizeof(c)) {
                    errx(8, "writing failed");
                }
            }
            ccount++;
        }
        else {
            if(fcount >= start && fcount <= end) {
                if(c == delim) {
                    if(fcount++ == end) {
                        continue;
                    }
                }
                if(write(1, &c, sizeof(c)) != sizeof(c)) {
                    errx(8, "writing failed");
                }
            }
        }
    }

    if(rs == -1) {
        errx(2, "failed reading");
    }

    exit(0);
}
