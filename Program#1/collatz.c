/*
Name: Riad Shash (Ray)
ID: N845Y337
CS540

Programming Assignment #1
This program uses the Collatz conjecture and applies it to any given positive integer n.

Collatz conjecture
n = n/2 if n is even
n = (3*n)+ 1 if n is odd

This ends when n converges to 1.
Uses the fork() system to call a child process
Parent process invokes the wait call to ensure process finishes

*/

#include <stdio.h> /* for printing the sequence */
#include <unistd.h> /* brings in the fork() command */
#include <sys/wait.h> /* for wait */
#include <stdlib.h> /* for the atoi library call */
#include <string.h>

int main(int argc, char *argv[]){
	printf("\n");

	//Checks if user provided a command line argument
	if (argc != 2){
		fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
		return 1;
	}

	pid_t pid;

	//converts a string to a int
	int n = atoi(argv[1]);

	//Checks if the user entered a legal value
	if (n <= 1 || n> 500000){
		fprintf(stderr, "Please enter a integer > 1 and <= 500,000\n");
		return 1;
	}

	/* fork a child process */
	pid = fork();

	//Check to see if creating a process failed
	if (pid < 0){
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	//Starts the child process
	else if (pid == 0){
		//Child Process
		int count = 1;			//used to know when to start new line
		printf("%d, ", n);

		while(n != 1){
			if(n%2 == 0){
				n = n/2;
			}
			else{
				n = (n * 3) +1;
			}
			count++;

			if (count%10 == 0){
				printf("%d\n", n);
			}
			else if (count%10 == 1 && n != 1){
				printf("%d, ", n);
			}
			else if (n == 1 ||  count%10 == 1){
				printf("%d\n", n);
			}
			else{
				printf("%d, ", n);
			}
		}
	}
	else{	//Parent process
		//Parent will wait for child to complete
		wait(NULL);
		printf("\n");
	}
	return 0;
}
