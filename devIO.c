// Aaron Liszt
// devIO.c
// Reads from and writes to dev files
// (NEED READ/WRITE PERMISSON)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(){
	int option = 0;		// Users selection number for main looping menu
	char buff[3];		// Small buffer for reading in option(int)
	char fileName[50];	// Buffer for filename or path
	char output[50];	// Buffer for file I/O
	FILE *userFile;		// File pointer
	int validName = 0;	// bool value to verify the user entered a valid filename

	while(option != 2){
		option = 0;
		printf("\n1. Enter a filename to write to\n");
		printf("2. Exit\n");
		printf("Enter your selection as '1' or '2'(without ''): ");
		fgets(buff, 3, stdin);	// get all input
		sscanf(buff, "%d", &option); // search the input for an int

		switch(option){
			case 1: // Read/Write from file
				validName = 0;
			
				while(validName != 1){	// Loop until the user enters a valid filename
					printf("\nEnter the name or path of the file to write to: ");
					scanf(" \n%s", &fileName);
					userFile = fopen(fileName, "r+");	
					if(userFile == NULL) //Check if file exists
						printf("Please enter a valid filename or path!\n");
					else
						validName = 1;
				}

				printf("Enter the what you would like to write/ouput from the file: ");
				fgets(output, 50, stdin);	// Get all input and put it into output
				scanf("%[^\n]%*c", output);	// Keep all user input including whitespace, ingnore any input after a '\n' character

				fseek(userFile, 0, SEEK_END);	// Set pointer to end of file
				fprintf(userFile, "%s", output);// Place whats in output at EOF
				printf("\nYour input was added to the file! Here is the new file content - \n\n");
				 
				char c;
				rewind(userFile);

				while(1){ // Print out the contents of userFile
					c = fgetc(userFile);
					if(c == EOF)
						break;

					printf("%c", c);
				}
				printf("\n\n --END OF FILE --\n");

				fclose(userFile);
				break;
			case 2: // EXIT
				printf("\nGood-bye!\n");
				break;

			default: // INVALID INPUT
				printf("\nPlease enter a valid int 1 or 2!\n");
		}
	}
}
