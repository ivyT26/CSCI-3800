#include "apue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main ()
{
	if (mkdir("/tmp/foo", 00777) < 0)
		err_sys("mkdir failed.\n");
	if (chdir("/tmp/foo") < 0)
		err_sys("chdir failed.\n");
	if (rmdir("/tmp/foo") < 0)
		err_sys("rmdir failed.\n");
	
	printf("removed /tmp/foo directory successfully.\n");
	
	DIR *dirStream;
	dirStream = opendir(".");
	if (dirStream == NULL) {
		printf("cannot open . directory.\n");
	}
	else if (readdir(opendir(".")) < 0) {
		printf("cannot read . directory.\n");
	} else {
		printf("read . directory successfully.\n");
	}
	
	dirStream = opendir("../foo");
	if (dirStream == NULL) {
		printf("cannot open ../foo directory.\n");
	}
	else if (readdir(opendir("../foo")) < 0) {
		printf("cannot read ../foo directory.\n");
	} else {
		printf("read ../foo directory successfully.\n");
	}

	dirStream = opendir("/tmp/foo");
	if (dirStream == NULL) {
		printf("cannot open /tmp/foo directory.\n");
	}	
	else if (readdir(opendir("/tmp/foo")) < 0) {
		printf("cannot read /tmp/foo directory.\n");
	} else {
		printf("read /tmp/foo directory successfully.\n");
	}		

	exit(0);
}
