/*
Name: Riad Shash (Ray)
ID: n845y337
CS540

Programming Assignment #2
This program again uses the Collatz conjecture and applies it to a given positive number.
A child process is created to produce the sequence and writes it into shared memory. Later the
parent process will print out the generated collatz sequence from shared memory.

Collatz conjecture:
n = n/2 if n is even
n = (3*n) + 1 if n is odd
This ends when n converges to 1.
*/

#include <stdio.h>      /* for printing the sequence    */
#include <unistd.h>     /* brings in the fork() command */
#include <sys/wait.h>   /* for wait system call */
#include <string.h>     /* for the strlen function */
#include <sys/shm.h>    /* for using shared memory */
#include <sys/mman.h>   /* for using shared memory */
#include <fcntl.h>      /* defines O_CREAT, O_RDWR, O_RDONLY */
#include <string.h>
#include <stdlib.h>	/* for the atoi library call */
#include <fcntl.h>

int main(int argc, char *argv[]){
	printf("\n");

	pid_t pid;                   	 /* 0 for child process, non-zero for parent */
	const int SIZE = 4096;       	 /* The size in bytes of shared memory       */
	const char *name = "collatz";    /* name of the shared memory object         */
	int shm_fd;              	 /* shared memory file descriptor            */
	void *ptr = NULL ;            	 /* ptr to the shared memory object          */

	//used to test the input argument
	char *testptr = NULL;		//indexes into the string of digits of argv[1]
	char ch;			//the symbol testptr points to
	char *digit_ptr = NULL;

	//if the user did not provide a argument
	if (argc != 2){
                fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
                return 1;
        }

	testptr = argv[1];			//starts at the beg. of argv[1]
	ch = *testptr;				//looks at the first char

	if (ch == '-'){
		fprintf(stderr, "Can not start with a negative sign!\n");
		return 1;
	}

	digit_ptr = testptr;

	//checks if the input consists of only digits
	while (ch != '\0' && ('0' <= ch && ch <= '9')){
		testptr++; /* move to the next char */
		ch = *testptr;
	}

	if (ch != '\0') /* if never reached the end*/{
		fprintf(stderr, "The input must consist of digits!\n");
		return 1;
	}

        //converts a string to a int
        int n = atoi(digit_ptr);

        //Checks if the user entered a legal value
        if (n <= 1 || n> 499999){
                fprintf(stderr, "Please enter a integer > 1 and <= 499,999\n");
                return 1;
        }

	/* open the shared memory object as if it were a file, but kept in memory*/
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* specify the fixed size in number of bytes */
	ftruncate(shm_fd, SIZE);

	/* memory map the shared memory object as a file the parent process will read */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	/* Fork a process */
	pid = fork();

	if (pid < 0)  /* error occurred */{
      		fprintf(stderr, "Fork Failed\n");
      		return 1;
   	}
	else if (pid == 0){
		/* open the shared memory object for reading and writing */
      		shm_fd = shm_open(name, O_RDWR, 0666);

      		/* keep the shared memory object to the size the parent wanted it */
      		ftruncate(shm_fd, SIZE);

      		/* memory map the shared memory object */
      		ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

      		/* write to the shared memory object as if it were a regular file */
		//Child Process
                int count = 1;                  //used to know when to start a new line
		char printstr[20];
                //printf("%d, ", n);
		snprintf(printstr, 20, "%d", n);
		strcat(printstr, ", ");
		sprintf(ptr,"%s",printstr);
		ptr += strlen(printstr);


		while(n != 1){
			//collatz algorithm
                        if(n%2 == 0){
                                n = n/2;
                        }
                        else{
                                n = (n * 3) +1;
                        }
                        count++;

                        if (count%10 == 0){
                               // printf("%d\n", n);
				snprintf(printstr, 20, "%d", n);
				strcat(printstr, "\n");
				sprintf(ptr,"%s",printstr);
				ptr += strlen(printstr);
                        }
                        else if (count%10 == 1 && n != 1){
                                //printf("%d, ", n);
				snprintf(printstr, 20, "%d", n);
				strcat(printstr, ", ");
				sprintf(ptr,"%s",printstr);
				ptr += strlen(printstr);
                        }
                        else if (n == 1 ||  count%10 == 1){
                                //printf("%d\n", n);
				snprintf(printstr, 20, "%d", n);
				strcat(printstr, "\n");
				sprintf(ptr,"%s",printstr);
				ptr += strlen(printstr);
                        }
                        else{
                                //printf("%d, ", n);
				snprintf(printstr, 20, "%d", n);
				strcat(printstr, ", ");
				sprintf(ptr,"%s",printstr);
				ptr += strlen(printstr);

                        }
                }
	}
	else{
		wait(NULL);			//waits for the child to finish
		shm_fd = shm_open(name, O_RDONLY, 0666);
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
		printf("%s\n",(char *)ptr);
		//removes the shared memory object
		shm_unlink(name);
	}
	//end of program
	return 0;
}