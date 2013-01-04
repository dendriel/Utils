/*
 * vitor.rozsa
 * 04/01/2013
 *
 * ref.: http://www.easysw.com/~mike/serial/serial.html#2_5_4
 */
/* Includes */
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/* Defines */
#define PORT "/dev/ttyUSB1"

// return values
#define ERROR -1
#define OK 0
#define CMD "testCommand\r"

/* Functions headers */
int port_open(char *device);
int port_close(int fd);
int stream_watch(int fd, char *data);
int stream_write(int fd, char data[]);

/*
 * Brief: Watch port until there is some data
 */
int main(void)
{
	int fd;
	char data[150];

	fd = port_open(PORT);
	if(fd < 0) {
		printf("main failed!\n");
		return ERROR;
	}

	stream_write(fd, CMD);

	sleep(1);

	if(stream_watch(fd, data) != OK ) {
		printf("Failed to read stream!\n");
	}
	else {
		printf("Data read from port: [%s]\n", data);
	}

	if(port_close(fd) < 0) {
		printf("Failed to close the port!\n");
		return ERROR;
	}

	return OK;
}


/*
 * Brief Open and configure serial port.
 * Param device The path to file that represents 
 *  the serial port.
 * Returns 0 if everything went fine, -1 otherwise.
 */

int port_open(char *device)
{
	int fd;
	struct termios options;

	if (device == NULL) {
		printf("Invalid device.\n");
		return ERROR;
	}

	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

	if(fd == -1) {
		printf("Failed to open port \"%s\".\n", device);
		return ERROR;
	}

	fcntl(fd, F_SETFL, 0);

    /* get the current options */
    tcgetattr(fd, &options);

    /* set raw input, 1 second timeout */
    options.c_cflag     |= (CLOCAL | CREAD);
    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag     &= ~OPOST;
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 10;

    /* set the options */
    tcsetattr(fd, TCSANOW, &options);

	return fd;
}

/*
 * Brief Close the specified port
 * Param fd The serial port descriptor
 * Return the result of close[?]
 */
int port_close(int fd)
{
	return close(fd);
}


/*
 * Brief Receive data
 */
int stream_watch(int fd, char *data)
{
	int len;

	len = read(fd, data, 150);
	if(len < 0) {
		return ERROR;
	}

	return OK;
}

/*
 * Brief Write data into the stream
 */
int stream_write(int fd, char data[])
{
	int n;

	n = write(fd, data, strlen(data));
	if (n < strlen(data)) {
		fputs("write() failed!\n", stderr);
		return ERROR;
	}

	return OK;
}
