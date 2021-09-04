#include <stdint.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>

void cfds(int c, ...) {
	va_list fds;
	va_start(fds, c);

	for(int i=0; i<c; i++) {
		int fd = va_arg(fds, int);
		if(fd != -1) {
			close(fd);
		}
	}

	va_end(fds);
}

int main(int argc, char* argv[]) {
    if(argc != 7) {
        errx(1, "invalid number of arguments");
    }


	int affix = open(argv[1], O_RDONLY),
		postfix = open(argv[2], O_RDONLY),
		prefix = open(argv[3], O_RDONLY),
		infix = open(argv[4], O_RDONLY),
		suffix = open(argv[5], O_RDONLY),
		crucifixus = open(argv[6], O_WRONLY|O_CREAT|O_TRUNC, 0644);

	if(-1 == affix || -1 == postfix ||
			-1 == prefix || -1 == infix ||
			-1 == suffix || -1 == crucifixus) {
		cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
		errx(1, "opening file failed");
	}

	uint16_t lAffix, lPostfix, lPrefix, lInfix, lSuffix;

	if(lseek(affix, 5, SEEK_SET) == -1 ||
			lseek(postfix, 5, SEEK_SET) == -1 ||
			lseek(prefix, 5, SEEK_SET) == -1 ||
			lseek(infix, 5, SEEK_SET) == -1 ||
			lseek(suffix, 5, SEEK_SET) == -1) {	
		cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
		errx(2, "lseek failed");
	}

	if(read(affix, &lAffix, sizeof(lAffix)) != sizeof(lAffix) ||
			read(postfix, &lPostfix, sizeof(lPostfix)) != sizeof(lPostfix) ||
			read(prefix, &lPrefix, sizeof(lPrefix)) != sizeof(lPrefix) ||
			read(infix, &lInfix, sizeof(lInfix)) != sizeof(lInfix) ||
			read(suffix, &lSuffix, sizeof(lSuffix)) != sizeof(lSuffix)) {
		cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
		errx(3, "reading failed");
	}

	if(lseek(affix, 16, SEEK_SET) == -1) {
		cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
		errx(2, "lseek failed");
	}

	uint16_t set[8];
	int rs;
	while((rs = read(affix, set, sizeof(set))) == sizeof(set)) {
		uint16_t eInfix;
		uint8_t ePrefix;
		uint32_t ePostfix;
		uint64_t eSuffix;

		if(lseek(postfix, set[0], SEEK_SET) == -1 ||
				lseek(prefix, set[2], SEEK_SET) == -1 ||
				lseek(infix, set[4], SEEK_SET) == -1 ||
				lseek(suffix, set[6], SEEK_SET) == -1) {	
			cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
			errx(2, "lseek failed");
		}

		while(set[1]-- && (rs = read(postfix, &ePostfix, sizeof(ePostfix)) == sizeof(ePostfix))) {
			if(write(crucifixus, &ePostfix, sizeof(ePostfix)) != sizeof(ePostfix)) {
				cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
				errx(4, "writing failed");
			}
		}

		if(rs != sizeof(ePostfix) && rs != 0) {
			cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
			errx(3, "reading failed");
		}
		
		while(set[3]-- && (rs = read(prefix, &ePrefix, sizeof(ePrefix)) == sizeof(ePrefix))) {
			if(write(crucifixus, &ePrefix, sizeof(ePrefix)) != sizeof(ePrefix)) {
				cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
				errx(4, "writing failed");
			}
		}

		if(rs != sizeof(ePrefix) && rs != 0) {
			cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
			errx(3, "reading failed");
		}
		
		while(set[5]-- && (rs = read(infix, &eInfix, sizeof(eInfix)) == sizeof(eInfix))) {
			if(write(crucifixus, &eInfix, sizeof(eInfix)) != sizeof(eInfix)) {
				cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
				errx(4, "writing failed");
			}
		}

		if(rs != sizeof(eInfix) && rs != 0) {
			cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
			errx(3, "reading failed");
		}

		while(set[7]-- && (rs = read(suffix, &eSuffix, sizeof(eSuffix)) == sizeof(eSuffix))) {
			if(write(crucifixus, &eSuffix, sizeof(eSuffix)) != sizeof(eSuffix)) {
				cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
				errx(4, "writing failed");
			}
		}

		if(rs != sizeof(eSuffix) && rs != 0) {
			cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
			errx(3, "reading failed");
		}
	}

	if(rs != sizeof(set) && rs != 0) {
		cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
		errx(3, "reading failed");
	}

	cfds(6, affix, postfix, prefix, infix, suffix, crucifixus);
	exit(0);
}
