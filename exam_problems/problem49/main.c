#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    if(argc < 2 || argc > 4) {
        errx(1, "invalid num of args");
    }

    if(strcmp(argv[1], "-d") == 0) {
        if(argc < 3) {
            errx(1, "invalid num of args");
        }

        char c;
        int rs;
        while((rs=read(0,&c,sizeof(c))) > 0) {
            bool toDelete = false;

            for(int i=0; i<(int)strlen(argv[2]); i++) {
                if(c == argv[2][i]) {
                    toDelete = true;
                    break;
                }
            }

            if(!toDelete) {
                if(write(1,&c,sizeof(c)) != sizeof(c)) {
                    errx(3, "writing failed");
                }
            }
        }

        if(rs == -1) {
            errx(2, "reading failed");
        }
    }
    else if(strcmp(argv[1], "-s") == 0) {
        if(argc<3) {
            errx(1, "invalid num of args");
        }

        char c, *p = NULL;
        int rs;
        
        while((rs=read(0,&c,sizeof(c))) > 0) {
            if(p!=NULL && (*p)==c) {
                continue;
            }
            else if(p==NULL || (*p)!=c) {
                p=NULL;
                for(int i=0; i<(int)strlen(argv[2]); i++) {
                    if(c == argv[2][i]) {
                        p = &argv[2][i];
                        break;
                    }
                }
            }


            if(write(1,&c,sizeof(c)) != sizeof(c)) {
                errx(3, "writing failed");
            }
        }

        if(rs == -1) {
            errx(2, "reading failed");
        }

    }
    else {
        if (argc != 3) {
            errx(1, "invalid number of args");
        }
        if(strlen(argv[1])!=strlen(argv[2])) {
            errx(4, "sets are not equal in size");
        }

        char c;
        int rs;

        while((rs=read(0,&c,sizeof(c))) > 0) {
            for(int i = strlen(argv[1]) - 1; i>=0; i--) {
                if(c == argv[1][i]) {
                    c = argv[2][i];
                    break;
                }
            }

            if(write(1,&c,sizeof(c)) != sizeof(c)) {
                errx(3, "writing failed");
            }
        }

        if(rs == -1) {
            errx(2, "reading failed");
        }
    }

    exit(0);    
}
