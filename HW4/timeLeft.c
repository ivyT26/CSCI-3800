#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

/*Name: Ivy Truong ID: 108972556 Class: CSCI 3800 Intro to Unix Programming*/

int main(){
	time_t t;	
	struct tm *tmp, *newtmp;
	char buf[1024]; /*buffer to process time structure and to print out in a nice format*/	
	char newDate[128]; /*buffer to store user input*/
	char val1[128];
	int day1, day2;
	int month1, month2;
	int year1, year2;
	int hours;
	int minutes;
	int seconds;

	time(&t); /*get the current time and date*/
	tmp = localtime(&t); /*converts the calendar time to local time including the time zones*/

	if (strftime(buf, 1024, "Current date and time: %F %A %r, %Z", tmp) == 0) {
		printf("buffer too small\n");
	} else {
		printf("%s\n", buf);
	} 

	printf("Please enter a target date in the future(mm/dd/yyyy):");
	scanf("%s", newDate);
	
	if (strptime(newDate, "%m/%d/%Y", newtmp) == NULL) {
		printf("Future date in illegal format\n");
		exit(1);
	}

	/*here check if the future date is greater than the current date*/
	/*here obtain the months, days, and years of the two dates*/
	strftime(val1, 128, "%Y", tmp);	
	/*printf("%s ", val1);*/
	/*convert char array to int*/
	sscanf(val1, "%d", &year1);
	/*printf("%d\n", year1);*/
	strftime(val1, 128, "%Y", newtmp);
	/*printf("%s ", val1);*/
	sscanf(val1, "%d", &year2);
	/*printf("%d\n", year2);*/

	strftime(val1, 128, "%m", tmp);
	sscanf(val1, "%d", &month1);
	/*printf("%d\n", month1);*/
	strftime(val1, 128, "%m", newtmp);
	sscanf(val1, "%d", &month2);
	/*printf("%d\n", month2);*/

	strftime(val1, 128, "%d", tmp);
	sscanf(val1, "%d", &day1);
	/*printf("%d\n", day1);*/
	strftime(val1, 128, "%d", newtmp);
	sscanf(val1, "%d", &day2);
	/*printf("%d\n", day2);*/

	if (year2 < year1) {
		printf("Future date is not greater than the current date based on years\n");
		exit(1);
	} else if (year1 == year2) {
		if (month2 < month1) {
			printf("Future date is not greater than current date based months of same year\n");
			exit(1);
		} else if (month1 == month2) {
			if (day2 < day1) {
				printf("Future date is not greater than current date based on days of same month and year\n");
				exit(1);
			}
		}
	}

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
	
	year1 = year2 - year1;
	year2 = year1 * 364;
	month1 = month2 - month1;
	month2 = month1 * 30;
	day1 = day2 - day1;
	day2 = day1 + month2 + year2;

	hours = 23 - hours;
	minutes = 59 - minutes;
	seconds = 59 - seconds;
	
	printf("Until target date: %d Days, %d Hours, %d Minutes, %d Seconds\n", day2, hours, minutes, seconds); 
	
	/*calculate total number of seconds*/
	minutes = minutes * 60;
	hours = hours * 60 * 60;
	day2 = day2 * 60 * 60 * 24;
	seconds = seconds + minutes + hours + day2;

	printf("Total number of seconds until target date: %d seconds\n", seconds);
	
	return 0;
}
