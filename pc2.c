// Aaron Liszt
// pc2.c
// Parent child process program with some use of signals

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void sigHandle() { printf("Child received frog\n"); }

main(){
	pid_t pid;		// Process ID
	int looping = 1;	// boolean looping int
        char input[15];		// user input string
	char *point;		// pointer to char in string(input)

	switch(pid = fork()){						// Split the process into parent/child
		case -1:						// Handel fork error
			printf("Fork failed!");
			break;
		case 0:							// Child process
			signal(SIGUSR1, sigHandle);			// Install custom user signal
			printf("Child waiting\n");
			for(;;){}					// Infinte Loop
			break;
		default:						// Parent process	
			sleep(1);					// Wait 1 sec for child process to execute		
			while(looping != 0){
				printf("Enter an input string: ");	
				fgets(input, 15, stdin);		// Read user input string
				printf("\n");
				
				if((point = strchr(input, '\n')) != NULL)// Remove new line character from input string
					*point = '\0';
				
				if(strcmp(input, "frog") == 0){		// If the user inputs frog signal the child process
					kill(pid, SIGUSR1);		
					sleep(1);			// Wait 1 sec for signal to execute
				}
				else if(strcmp(input, "quit") == 0) 	// If the user inputs quit exit the loop
					looping = 0;
				else					
					printf("Parent: <%s> is ok!\n", input);
			}
			printf("bye!\n");
			break;
	}
}
