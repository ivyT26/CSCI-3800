#include "apue.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>

/*Name: Ivy Truong ID: 108972556 Class: CSCI 3800 Intro to Unix Programming*/

static void customSignal(int); /*signal handler to handle signals*/

void daysLeft(); /*function called to display time info*/

/*global variables*/
int alrmnum = 0;
char quit = 'n'; /*keeps track of user choice if they want to continue program*/
int delay = 3; /*set alarm delay to 3 seconds, every thirs time set it to number greater than equal to 8 seconds*/

int main(){

	/*resource: https://stackoverflow.com/questions/4420163/problem-with-signal-alarm-in-unix*/
	alarm(1);

	while (quit == 'n') {
		/*call signals here if captured*/
		if (signal(SIGINT, customSignal) == SIG_ERR)
			err_sys("can't catch SIGINT");
		else if (signal(SIGALRM, customSignal) == SIG_ERR)
			err_sys("can't catch SIGALRM");
	
	} /*end of while loop*/
	
	return 0;
}

static void customSignal(int signo) {

	static int numInterrupts = 0; /*keeps track of how many interrupts there have been, when it reaches 3 the program will terminate*/
	static int numAlarms = 0; /*keeps track of how many alarms have been called*/

	if (signo == SIGINT) {
		alrmnum = 1;
		numInterrupts += 1;
		if (numInterrupts > 3) {
			quit = 'y';
			return;
		}
	} else if (signo == SIGALRM) {
		alrmnum = 0;
		numAlarms += 1;
		if ((numAlarms+1) % 3 == 0) {
			/*printf("Longer delay\n");*/
			delay = 15;
		} else {
			/*printf("Shorter delay\n");*/
			delay = 3;
		}
	} else {
		err_dump("received signal %d\n", signo);
	}
	
	daysLeft();
}

void daysLeft() {
	time_t t, t1, t2;	
	struct tm *tmp, *dectmp, *jultmp;
	char buf[1024]; /*buffer to process time structure and to print out in a nice format*/	
	char val1[128];
	int day1 = 0;
	int month1 = 0;
	int year1 = 0;
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	static int seqNum = 0;
	char *alarmType[] = {"SIGALRM", "SIGINT"};
	
	printf("%d By %s:\n", seqNum, alarmType[alrmnum]);

	time(&t); /*get the current time and date*/
	tmp = localtime(&t); /*converts the calendar time to local time including the time zones*/

	if (strftime(buf, 1024, "\tCurrent date and time: %F %A %r, %Z", tmp) == 0) {
		printf("buffer too small\n");
	} else {
		printf("%s\n", buf);
	} 

	/*here obtain the months, days, and years of the current date*/
	strftime(val1, 128, "%Y", tmp);	
	/*printf("%s ", val1);*/
	/*convert char array to int*/
	sscanf(val1, "%d", &year1);
	/*printf("%d\n", year1);*/

	strftime(val1, 128, "%m", tmp);
	sscanf(val1, "%d", &month1);
	/*printf("%d\n", month1);*/

	strftime(val1, 128, "%d", tmp);
	sscanf(val1, "%d", &day1);
	/*printf("%d\n", day1);*/

	/*convert to days, hours, minutes, and seconds*/
	/*here obtain the hours, minutes, and seconds of the dates, assume that the future date starts at midnight*/
	strftime(val1, 128, "%H", tmp); /*obtains hours in 24 hour format*/
	sscanf(val1, "%d", &hours);
	/*printf("%d\n", hours);*/
	strftime(val1, 128, "%M", tmp);
	sscanf(val1, "%d", &minutes);
	/*printf("%d\n", minutes);*/
	strftime(val1, 128, "%S", tmp);
	sscanf(val1, "%d", &seconds);
	/*printf("%d\n", seconds);*/
	
	/*copy target dates in correct format*/

	if (alrmnum == 0) {
		time(&t1);
		dectmp = localtime(&t1);
		dectmp->tm_year = 2022;
		dectmp->tm_mday = 25;
		dectmp->tm_mon = 12;
 	
		year1 = dectmp->tm_year - year1;
		year1 = year1 * 364;
		month1 = dectmp->tm_mon - month1;
		month1 = month1 * 30;
		day1 = dectmp->tm_mday - day1;
		day1 = day1 + month1 + year1;

		hours = 23 - hours;
		minutes = 59 - minutes;
		seconds = 59 - seconds;
		
		printf("\tUntil 2022 Christmas Day: %d Days, %d Hours, %d Minutes, %d Seconds\n", day1, hours, minutes, seconds);
		
		/*calculate total number of seconds*/
		minutes = minutes * 60;
		hours = hours * 60 * 60;
		day1 = day1 * 60 * 60 * 24;
		seconds = seconds + minutes + hours + day1;

		printf("\tTotal number of seconds: %d seconds\n", seconds);
		alarm(delay); /*set alarm again here*/
	} else if (alrmnum == 1) {
		time(&t2);
		jultmp = localtime(&t2);
		jultmp->tm_year = 2022;
		jultmp->tm_mday = 4;
		jultmp->tm_mon = 7;
	
		year1 = jultmp->tm_year - year1;
		year1 = year1 * 364;
		month1 = jultmp->tm_mon - month1;
		month1 = month1 * 30;
		day1 = jultmp->tm_mday - day1;
		day1 = day1 + month1 + year1;

		hours = 23 - hours;
		minutes = 59 - minutes;
		seconds = 59 - seconds;

		printf("\tUntil 2022 Independence Day: %d Days, %d Hours, %d Minutes, %d Seconds\n", day1, hours, minutes, seconds);
		
		/*calculate total number of seconds*/
		minutes = minutes * 60;
		hours = hours * 60 * 60;
		day1 = day1 * 60 * 60 * 24;
		seconds = seconds + minutes + hours + day1;

		printf("\tTotal number of seconds: %d seconds\n", seconds);
		
		do {
		printf("Quit (y/n)?:");
		scanf(" %c", &quit);
		quit = tolower(quit);
		} while (quit != 'n' && quit != 'y');
	}
	
	seqNum += 1;

	return;
}
