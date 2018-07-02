// Aaron Liszt
// xDate.c
// uses execv() to call the 'date' linux command

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

main(){
	int rVal;		// return value of execv
	char *cmd[] = {"date"};	// store command to be called
	
	printf("Ready to call exec()\n");
	printf("Process ID: %d\n", getpid()); // Get/print process id
	rVal = execv("/bin/date", cmd); // call execv with path to date command and the previosly initialized argument

	if(rVal == -1)
		printf("An error occured calling 'date'");
}
