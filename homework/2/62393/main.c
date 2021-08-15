#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>

// enum of error codes
enum ERR {
    SUCCESS=0,
    E_READ,
    E_WRITE,
    E_OP,
    E_SEQ,
    E_FD,
    E_ARG,
    E_SLEEP
};

// custom error codes and messages
void errmsg(int code) {
    switch(code) {
        case SUCCESS:
            exit(code);
        case E_READ:
            errx(code, "failed reading from stream or file");
        case E_WRITE:
            errx(code, "failed writing to stream or file");
        case E_OP:
            errx(code, "invalid operation");
        case E_SEQ:
            errx(code, "invalid sequence of bytes");
        case E_FD:
            errx(code, "failed opening file descriptor");
        case E_ARG:
            errx(code, "invalid number of arguments");
        case E_SLEEP:
            errx(code, "thread execution suspension failed");
        default:
            errx(code, "unknown error");
    }
}

// func to simulate the message interruptions
int suspend_thread(double time) {
	struct timespec sleepTime;
	sleepTime.tv_sec = (int)time;
	sleepTime.tv_nsec = (time-(int)time) * 1e9;

    return nanosleep(&sleepTime, NULL);
}

// func to output the data to the given file descriptor
// in the required format
int output_formatted(int fd, uint16_t id, ...) {

	// initialize arg list
	va_list args;
	va_start(args, id);

	if(0x0001 == id) {

        // extract the arguments from the list
		uint16_t slots = va_arg(args, int);
		double tempC = va_arg(args, double);
		double time = va_arg(args, double);

		va_end(args);
		
		// output in the required format to given descriptor
		if(dprintf(fd, "[%.3lf] <state> temp: %.2lf°C, slots:", time, tempC) == -1) {
			return -1;
        }
		for(unsigned int i = 1; i <= sizeof(slots)*8; i++) {
            // least significant bit represents the slot
			if(dprintf(fd, " %d[%c]", i, ((slots & 1) ? 'X' : ' ')) == -1) {
				return -1;
            }
			
            // shift bits to the right
            slots >>= 1;
		}
		if(dprintf(fd, "\n") == -1) {
			return -1;
        }
	}
	else if (0x0002 == id) {

        // extract the arguments from the list
		uint8_t slot = va_arg(args, int);
		char* text = va_arg(args, char*);
		double time = va_arg(args, double);

		va_end(args);

		// output in the required format to given descriptor
		if(dprintf(fd, "[%.3lf] <slot text> slot %d: %s\n", time, slot + 1, text) == -1) {
			return -1;
        }
	}
	else {
		va_end(args);
		return -1;
	}

	return 0;
}

// func to read from toaster/file
// and write to file/stdout and stderr
int rw_toaster(int fd_in, int fd_out, struct timeval start) {

    // current and starting time, timestamp from file if replaying
    double curTime,
           startTime = start.tv_sec + start.tv_usec/1e6,
           timestamp;
	
	uint16_t id;
    int read_status = 0;

    // start reading from input descriptor
	while((read_status = read(fd_in, &id, sizeof(id))) == sizeof(id)) {

        // get current time of execution
        struct timeval cur;
        gettimeofday(&cur, NULL);
        curTime = cur.tv_sec + cur.tv_usec/1e6 - startTime;

		// if first two bytes indicate <state>
		if(0x0001 == id) {

			uint16_t slots;
			uint32_t tempF;

			// read 2 bytes for the slots mask
			// 4 bytes for the temperature
			// and if it's a 'replay' operation
			// read 8 bytes for the timestamp
			if(read(fd_in, &slots, sizeof(slots)) != sizeof(slots) ||
                    read(fd_in, &tempF, sizeof(tempF)) != sizeof(tempF) ||
                    (1 == fd_out && read(fd_in, &timestamp, sizeof(timestamp)) != sizeof(timestamp))) {
				return E_READ;
            }

			// convert from F to C
			double tempC = tempF;
			tempC = tempC / 100 - 273.15;
			
			// replay
			if(1 == fd_out) {

                // set required time to interrupt
                double toSleep = timestamp - curTime;

                // simulate interruption and update previous timestamp
                if(toSleep > 0 && suspend_thread(toSleep) == -1) {
                    return E_SLEEP;
                }
                
                curTime = timestamp;

                // time after interruption
                // uncomment below to check the error difference
                // gettimeofday(&cur, NULL);
                // actual = cur.tv_sec + cur.tv_usec/1e6 - startTime;
                // fprintf(stderr,"%.3lf\n", actual);
            }

			// write data to output descriptor
			if(write(fd_out, &id, sizeof(id)) != sizeof(id) ||
					write(fd_out, &slots, sizeof(slots)) != sizeof(slots) ||
					write(fd_out, &tempF, sizeof(tempF)) != sizeof(tempF) ||
                    // if we are recording write the timestamp as well
					(1 != fd_out && write(fd_out, &curTime, sizeof(curTime)) != sizeof(curTime))) {
				return E_WRITE;
			}
			
			// stderr output
			if(output_formatted(2, id, slots, tempC, curTime) == -1) {
                return E_WRITE;
            }
		}
		// if first two bytes indicate <slot_text>
		else if (0x0002 == id) {

			uint8_t slot;
			char text[13];

			// read 1 byte for the slot
			// 13 bytes for the text
			// and if it's a 'replay' operation
			// read 8 bytes for the timestamp
			if(read(fd_in, &slot, sizeof(slot)) != sizeof(slot) ||
                    read(fd_in, &text, sizeof(text)) != sizeof(text) ||
                    (1 == fd_out && read(fd_in, &timestamp, sizeof(timestamp)) != sizeof(timestamp))) {
				return E_READ;
            }

			// replay
			if(1 == fd_out) {

                // set required time to interrupt
                double toSleep = timestamp - curTime;
                
                // simulate interruption and update last timestamp
                if(toSleep > 0 && suspend_thread(toSleep) == -1) {
                    return E_SLEEP;
                }

                curTime = timestamp;

                // time after interruption
                // uncomment below to check the error difference
                // gettimeofday(&cur, NULL);
                // actual = cur.tv_sec + cur.tv_usec/1e6 - startTime;
                // fprintf(stderr,"%.3lf\n", actual);
			}

			// write data to output descriptor
			if(write(fd_out, &id, sizeof(id)) != sizeof(id) ||
					write(fd_out, &slot, sizeof(slot)) != sizeof(slot) ||
					write(fd_out, &text, sizeof(text)) != sizeof(text) ||
                    // if we are recording write the timestamp as well
					(1 != fd_out && write(fd_out, &curTime, sizeof(curTime)) != sizeof(curTime))) {
				return E_WRITE;
            }
			
			// stderr output
			if(output_formatted(2, id, slot, text, curTime) == -1) {
                return E_WRITE;
            }
		}
		else {
			// error number for invalid sequence of bytes
			return E_SEQ;
		}
	}

    if(sizeof(id) != read_status && 0 != read_status) {
        return E_READ;
    }

	return 0;
}

int main(int argc, char* argv[]) {

    // get program starting time;
    struct timeval start;
    gettimeofday(&start, NULL);

	// check number of arguments
	if(argc < 3) {
        errmsg(E_ARG);
	}

	int result_code, fd;

	if(strcmp(argv[1], "record") == 0) {

		// open file descriptor for writing
		// create if file doesn't exist
		// else wipe it's contents
		if((fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH))==-1) {
			errmsg(E_FD);
		}

		// extract and write information to file
		result_code = rw_toaster(0, fd, start);
	}
	else if(strcmp(argv[1], "replay") == 0) {

		// open file descriptor for reading:
		if((fd = open(argv[2], O_RDONLY)) == -1) {
			errmsg(E_FD);
		}

		// extract information from file and output to stdin
		result_code = rw_toaster(fd, 1, start);
	}
	else {
		errmsg(E_OP);
	}

	close(fd);
	errmsg(result_code);
}
