#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <termios.h>

#define BUFFLEN 8

int main(int argc, char** argv) {

	if(argc < 2) {
		printf("usage: <serial port>");
		return -1;
	}
	//char buff[BUFFLEN];
	char rx = '\0';
	struct termios nts;
    struct termios ots;
    printf("reading from %s\n", argv[1]);	
	int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
	if( fd == -1) {
		printf("Error opening serial port : %d: %s\n", errno, strerror(errno));
		return -1;
	}
    tcgetattr(fd, &ots);
    memset(&nts, '\0', sizeof(nts));
	nts.c_cflag = B9600 | CS8 | CLOCAL;	
	//memset(buff, '\0', BUFFLEN);
	
	tcflow(fd, TCOON);
	int ret = read(fd, &rx, 1);
	while(ret != -1 || errno == 35) {
		if( ret != -1) printf("%c", rx);
		ret = read(fd, &rx, 1);
	}
	printf("an error occured: %d : %s\n", errno, strerror(errno));
	tcsetattr(fd, TCSADRAIN, &ots);
	close(fd);
    return 0;
}
