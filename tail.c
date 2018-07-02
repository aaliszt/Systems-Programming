// Aaron Liszt
// tail.c
// works the same as linux's tail command

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

main(int ac, char *av[]){
	if(ac > 4 || ac == 3 || ac < 2){
		fprintf(stderr, "usage: %s filename", *av);
                exit(1);
	}

	FILE *file1;
	int NUM_LINES = 9; //Number of lines at for EOF to print (0-9)
	if(ac == 2){
		file1 = fopen(av[1], "r");
		if(file1 == NULL)
			printf("ERROR: File did not open properly\n");			
	}
	else{
		file1 = fopen(av[3], "r");
                if(file1 == NULL)
                        printf("ERROR: File did not open properly\n");
		
		if((int) av[2] - '0' > 9 && av[2] - '0' < 0)	// Handle converson of users num for single digit number
			NUM_LINES = (int) av[2] - '0';
		else{						// Handle conversion of users num for double digit number 
			char *num = av[2];
			NUM_LINES = atoi(num);
		}
		--NUM_LINES; // Decrement the number of lines the user entered because we are looping staring from 0
	}	

	int count = 0;
	int start_point = ftell(file1);	// Beggining of file
	fseek(file1, 0, SEEK_END);	// Go to EOF
	int end_point = ftell(file1);	// End of file 

	//printf("%d\n", start_point);
	//printf("%d\n", end_point);
	
	int current_point; // current point in file
	char current_char; // current character in file
	int last_ten;	   // marks the file point where the last 10 lines start	

	current_point = end_point;
	int offset = -2;		// Offset must be -2 to avoid extra /n at EOF
	fseek(file1, offset, SEEK_END);
	
	while(current_point > start_point+1){ 	// Loop from EOF to the start	
		if(current_point < start_point) // If current reaches start we must exit
			break;
		
		current_char = fgetc(file1); 	// Get character at current file position
		if(current_char == '\n')
			count++;
		
		if(count == NUM_LINES)		// If we reach the NUM_LINES the user want mark this point
			last_ten = ftell(file1);		

		--current_point;
		--offset;
		fseek(file1, offset, SEEK_END);
	}

	char c;
	fseek(file1, 0, SEEK_SET);
	fseek(file1, last_ten-1, SEEK_SET); 	// Set pointer to last 10 lines or user specified NUM_LINES

	while(!feof(file1)){			// Print out the number of lines the user wants
		c = fgetc(file1);
		if(feof(file1))
			break;

		printf("%c", c);
	}
	//printf("COUNT : %d\n", count);
	fclose(file1);
}

