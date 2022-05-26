#include "apue.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/*Name: Ivy Truong, ID: 108972556, Class: CSCI 3800-UNIX Programming*/

static void	sig_int(int);		/* our signal-catching function */

int
main(void)
{
	char	buf[MAXLINE];	/* from apue.h */
	pid_t	pid;
	int	status;
	char*   pbuf = malloc(MAXLINE * sizeof(char)); /*used to parse through char array and store as char* */
	char*   rest[MAXLINE]; /*used to store strings as char* in the array*/	

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal error");

	printf("%% ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0'; /* replace newline with null */
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {		/* child */
			/*need to convert everything in the buf variable to a char* and store it in a char* array*/
			/*strtok will take the source and save it as a char* null terminated string in the destination*/
			pbuf = strtok(buf, " "); /*gets first token found in string separated by the delimiter*/
			printf(pbuf);
			printf("\n");
			rest[0] = pbuf;
			/*copies the buffer into a new variable in char* form*/
			/*strcpy(rest, buf);*/
			int i = 1;
			pbuf = strtok(NULL, " ");
			/*loops through all tokens, or strings separated by spaces*/
			/*source came from tutorialspoint.com on using strtok*/
			while(pbuf != NULL) {
				rest[i] = pbuf;
				printf(rest[i]);
				++i;
				pbuf = strtok(NULL, " ");
			}
			/*strtok(NULL, delimiter) gets the next token of the previous string used*/
			/*execvp acces char* for first argument, then char*[] for second argument*/
			execvp((char*)rest[0], (char**)rest);
			err_ret("couldn't execute: %s", buf);
			exit(127);
		}

		/* parent */
		if ((pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");
		printf("%% ");
	}
	free(pbuf);
	exit(0);
}

void
sig_int(int signo)
{
	printf("interrupt\n%% ");
}
