# CSCI-3800

## Elective Class: Intro to UNIX Programming

This repository contains the homework assignments and labs from the Intro to UNIX Programming course.  
This course was taken at the University of Colorado Denver, Spring 2022. Professor: Sung Nam.  

All assignments use C as the main programming language.  

<b>Homeworks</b>  
HW1: Learned about various basic Linux commands and recorded the execution of those commands on the command line using scripts.      
HW2: Using command line and shell executable file write a set of Linux commands that reads a file and outputs the first 10 most frequent words in the file.  
HW3: Simulating dup2 command using file descriptors in C and learn about commands using file descriptors, situations when making, deleting, and accessing files, and fflush and fsync functionalities.  
HW4: Create a C program that calculates how many days, hours, minutes, and seconds it takes from the current date to the future target date using time functions. Also learned about forks and system functions.  
HW5: Learned about signal handlers, strtok command, reentrant functions, bringing executed jobs to the background and foreground, and threads vs processes. Also created a C program that simulates two processes taking turns to change the same file using signals.  

<b>Labs</b>  
Lab1: Created a C program that simulates a simple Linux shell. This does not handle commands with redirection.  
Lab2: Created a C program that expands on Lab1 and handles redirection commands. Also learned about how directory trees are read in Linux using C.  
Lab3: Created a C program that expands on HW4 using signals. The program will run two different types of commands. By default, the program will calculate the number of days left until Christmas Day every 3 or 15 seconds using alarm signals. If the user inputs an interrupt in the command line, the program will catch the interrupt signal and calculate the number of days left until Independence Day. The program stops once 3 interrupts have been inputted by the user or if the user chooses to quit.  
