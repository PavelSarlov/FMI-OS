#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    
    bool ln = false;
    int arg = 1;

    if(arg<argc && strcmp(argv[arg], "-n") == 0) {
        ln=true;
        arg++;
    }

    if(arg == argc) {
        char c;
        int rs;
        int lc = 1;
        bool lp = ln;
        
        while((rs=read(0,&c,sizeof(c))) == sizeof(c)) {
            if(lp) {
                printf("\t%d ", lc++);
                lp=false;
            }

            printf("%c", c);
            
            if(c=='\n' && ln) {
                lp=true;
            }
        }

        if(rs == -1) {
            errx(1, "failed reading");
        }
    }
    else {
        int lc = 1;

        for(int i=arg; i<argc; i++) {
            int fd;

            if(strcmp(argv[i],"-")==0) {
                fd = 0;
            }
            else if((fd=open(argv[i], O_RDONLY))==-1) {
                err(errno, NULL);
            }

            char c;
            int rs;
            bool lp = ln;

            while((rs=read(fd,&c,sizeof(c))) == sizeof(c)) {
                if(lp) {
                    printf("\t%d ", lc++);
                    lp=false;
                }

                printf("%c", c);

                if(c=='\n' && ln) {
                    lp=true;
                }
            }

            if(rs == -1) {
                if (fd!=0) {
                    close(fd);
                }
                errx(1, "failed reading");
            }

            if(fd!=0) {
                close(fd);
            }
        }
    }

    exit(0);
}
