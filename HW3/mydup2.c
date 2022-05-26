#include "apue.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		err_sys("not enough arguments.\n");		
	}
	
	/*get the file descriptor*/
	int oldfd = open(argv[2], O_RDONLY);
	
	if (oldfd < 0) {
		err_sys("cannot get file descriptor, path does not exist.\n");
	}
	printf("old file descriptor: %d\n", oldfd);
	printf("new file descriptor requested: %d\n", atoi(argv[1]));

	/*use fcntl to simulate dup2*/
	/*duplicate the old file descriptor, then set the new file descriptor that is unopened and greater than or equal to the third argument. returns the new file descriptor.*/
	int val = fcntl(oldfd, F_DUPFD, atoi(argv[1]));	
	
	if (val < 0) {
		err_sys("cannot duplicate file descriptor.\n");
	}
	
	char *buf = malloc(128 * sizeof(char));
	char *num = malloc(32 * sizeof(char));
	sprintf(num, "%d", val);
	strcpy(buf, "/proc/self/fd/");
	strcat(buf, num);	
	
	printf("path of new file descriptor: %s\n", buf);
	
	int check = readlink(buf, buf, 128);
	if (check < 0) {
		printf("error in reading link.\n");
	}	
	
	printf("new fd file path: %s\n", buf);	

	free(buf);
	free(num);
	exit(0); 

}
