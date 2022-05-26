#include "apue.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

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
	int 	inputOnly = 0; /*0 = no input redirection, 1 = only input redirection, 2 = input output redirection*/

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal error");

	printf("%% ");	/* print prompt (printf requires %% to print %) */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0'; /* replace newline with null */
		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {		/* child */
			/*fork a grandchild process, just in case multiple commands on the same line need to be executed*/
			/*printf("child process made.\n");*/
		
			/*need to convert everything in the buf variable to a char* and store it in a char* array*/
			/*strtok will take the source and save it as a char* null terminated string in the destination*/
			pbuf = strtok(buf, " "); 
			/*gets first token found in string separated by the delimiter*/
			/*printf(pbuf);*/
			/*printf("\n");*/
			rest[0] = pbuf;
			/*copies the buffer into a new variable in char* form*/
			/*strcpy(rest, buf);*/
			int i = 1;
			pbuf = strtok(NULL, " ");
			/*printf(pbuf);*/
			/*printf("\n");*/
			/*loops through all tokens, or strings separated by spaces*/
			/*source came from tutorialspoint.com on using strtok*/
			int fd = -1;
			char temp[10];
			while(pbuf != NULL) {
				/*printf(pbuf);*/
				/*printf("\n");*/
				strcpy(temp, pbuf);
				/*printf(temp);*/
				/*printf(" \n");*/
				/*check here if one of the arguments is a redirection symbol*/
				if (*pbuf == '<') {
					pbuf = strtok(NULL, " ");
					/*printf("input redirection \n");*/
					fd = open(pbuf, O_RDONLY);
					dup2(fd, 0); /*redirect input to file instead of a program variable*/
					/*the commands processed should be everything before the redirection symbol*/	
					/*printf(pbuf);*/
					/*printf("\n");*/
					inputOnly = 1;
					/*there may be more components to the command, so this is here so input output redirection combo commands can be handled*/
					pbuf = strtok(NULL, " ");
				} else if (temp[0] == '>' && temp[1] == '&') {
					pbuf = strtok(NULL, " ");
					/*printf("error redirection \n");*/
					fd = open(pbuf, O_WRONLY);
					dup2(fd, 2);
					break;
				} else if (*pbuf == '>') {
					pbuf = strtok(NULL, " ");
					/*printf("output redirection \n");*/
					fd = open(pbuf, O_WRONLY);
					dup2(fd, 1); /*redirect output to file instead of terminal*/
					break;
				} else {
					if (inputOnly != 1) { /*do not add any more to the command if there is more after input redirection*/
						rest[i] = pbuf;
						++i;
						pbuf = strtok(NULL, " ");
					}
				}
			}
			/*strtok(NULL, delimiter) gets the next token of the previous string used*/
			/*execvp acces char* for first argument, then char*[] for second argument*/
			int error = 0;
			error = execvp((char*)rest[0], (char**)rest);
			if (fd > -1) {
				close(fd);
			} 
			if (error < 0) {
				err_ret("couldn't execute: %s", buf);
				exit(127);
			}
			exit(0);
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
