// Aaron Liszt
// pc.c
// Simple parent child process program

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

main(){
	int parentID = getpid();
	pid_t pid;
	
	printf("PID: %d \nKSU Main Campus\n", parentID);		// Print out parent PID
	
	switch(pid = fork()){						// Split the process into parent/child
		case -1:
			printf("Fork failed!");
			break;
		case 0:							// Child process
			printf("KSU Regional Campuses\nAshtabula\n");	// Print all regional campuses
			printf("East Liverpool\n");
			printf("Geauga Campus\n");
			printf("Salem\n");
			printf("Stark\n");
			printf("Trumbull\n");
			printf("Tuscarawas\n");
			//printf("%d\n", getpid());
			break;
		default:						// Parent process
			wait((int *)0);					// Wait for child process
			parentID = getpid();				// Get parentID again
			printf("Parent process resumed, PID: %d \n", parentID);
			printf("Exiting...\n");
			break;
	} 
}
