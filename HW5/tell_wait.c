#include "apue.h"
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/*Ivy Truong; 108972556; CSCI 3800 Intro to UNIX Programming*/

/*have to use write instead of fwrite because write it is atomic (system call)*/

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;
static void sig_usr(int signo);

void TELL_WAIT(void);
void TELL_PARENT(pid_t pid);
void TELL_CHILD(pid_t pid);
void WAIT_PARENT(void);
void WAIT_CHILD(void);
void incNum();

int maxCount = 95;
int currCount = 0;

/*FILE* file;*/
int fd;

int main() {
	
	/*initialize file*/
	char buffer[] = "0";
	/*file = fopen("counter.txt", "w");*/
	/*fwrite(buffer, sizeof(buffer), 1, file);*/
	/*fclose(file);*/
	fd = open("counter.txt", O_WRONLY | O_CREAT, 00777);
	write(fd, buffer, sizeof(buffer));
	close(fd);

	pid_t pid;	
	struct timespec delay;
	delay.tv_sec = 1;
	delay.tv_nsec = 10000;

	TELL_WAIT();
	
	/*open file for reading and writing*/
	/*need to open file before fork or else the processes will open two different instances of the same file*/
	/*file = fopen("counter.txt", "w+");*/
	fd = open("counter.txt", O_RDWR, 00744);
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	}
	
	
	if (pid == 0) {
		/*child process*/
		while (currCount < maxCount) {
		printf("child process incrementing file counter\n");
		/*nanosleep(&delay, NULL);*/
		incNum();
		TELL_PARENT(pid);
		/*need delay just in case wait_child and wait_parent are called at the same time*/
		nanosleep(&delay, NULL);
		WAIT_PARENT();
		/*printf("after waiting for parent\n");*/
		}
	} else {
		/*parent process*/
		do {
		WAIT_CHILD();
		/*printf("after waiting for child\n");*/
		printf("parent process incrementing file counter\n");
		/*nanosleep(&delay, NULL);*/
		incNum();
		TELL_CHILD(pid);
		nanosleep(&delay, NULL);
		} while (currCount < maxCount);
	}

	close(fd);
	
	return 0;
}

void incNum() {
	char buffer[10];
	/*set/reset file pointer to beginning of file*/
	lseek(fd, 0, SEEK_SET);
	/*read the current count*/
	/*fread(buffer, sizeof(buffer), 1, file);*/
	read(fd, buffer, sizeof(buffer));
	/*convert string to integer*/
	sscanf(buffer, "%d", &currCount);
	printf("\tCount read from file: %d\n", currCount);
	/*increment count*/
	currCount += 1;
	/*convert back to string*/
	sprintf(buffer, "%d", currCount);
	/*printf("Current count incremented: %s\n", buffer);*/
	/*return pointer back to the beginning of the file*/
	lseek(fd, 0, SEEK_SET); /*2nd parameter is the offset, 3rd parameter is where to move the pointer*/
	/*write new results to the file*/
	/*fwrite(buffer, sizeof(buffer), 1, file);*/
	write(fd, buffer, sizeof(buffer));
}

static void
sig_usr(int signo)
{
    sigflag = 1;
}

void
TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_msg("signal(SIGUSR1) error");
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        err_msg("singal(SIGUSR2) error");
    }

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        err_msg("SIG_BLOCK error");
    }
}

void
TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);
}

void
WAIT_PARENT(void)
{
    while (sigflag == 0) {
        sigsuspend(&zeromask);
    }
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_msg("SIG_SETMASK error");
    }
}

void
TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}

void
WAIT_CHILD(void)
{
    while (sigflag == 0) {
        sigsuspend(&zeromask);
    }
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_msg("SIG_SETMASK error");
    }
}
