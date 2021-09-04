#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdarg.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

void cfds(int c, ...) {
	va_list fds;
	va_start(fds, c);

	for(int i=0; i<c; i++) {
		int fd = va_arg(fds, int);
		if(-1 != fd) {
			close(fd);	
		}
	}

	va_end(fds);
}

int power(int a, int p) {
	int result = 1;
	
	for(int i=0; i<p; i++) {
		result*=a;
	}

	return result;
}

int main(int argc, char* argv[]) {

	if(argc != 4) {
		errx(1, "invalid number of args");
	}

	int scl = open(argv[1], O_RDONLY),
		sdl = open(argv[2], O_RDONLY),
		out = open(argv[3], O_WRONLY|O_CREAT|O_TRUNC, 0644);

	if(-1 == scl || -1 == sdl || -1 == out) {
		cfds(3,scl,sdl,out);
		errx(2, "opening file failed");
	}

	uint16_t pos;
	int rs;
	while((rs = read(scl, &pos, sizeof(pos))) == sizeof(pos)) {
		if(lseek(sdl, pos/8, SEEK_SET) == -1) {
			cfds(3,scl,sdl,out);
			errx(4, "lseek failed");
		}

		uint8_t byte, mask = power(2, pos%8);
		if(read(sdl, &byte, sizeof(byte)) != sizeof(byte)) {
			cfds(3,scl,sdl,out);
			errx(5, "reading failed");
		}

		if((byte & mask) && write(out, &pos, sizeof(pos)) != sizeof(pos)) {
			cfds(3,scl,sdl,out);
			errx(5, "writing failed");
		}
	}

	if(sizeof(pos) != rs && 0 != rs) {
		cfds(3,scl,sdl,out);
		errx(3, "reading failed");
	}
	
	cfds(3,scl,sdl,out);
	exit(0);
}
