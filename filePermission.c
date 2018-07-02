// Aaron Liszt
// filePermission.c
// Displays file permissions for a specified file

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

main(int ac, char *av[]){

	if ( ac != 2 ){
                printf("usage: %s filename \n", *av);
		exit(1);
        }

	struct stat filePermission;

	stat(av[1], &filePermission);

	printf("\n----- Permissions for %s -----\n", av[1]);

	if(S_ISDIR(filePermission.st_mode))		// Check if argument 1 is a Directory
		printf("d");
	else
		printf("-");

	if(filePermission.st_mode & S_IRUSR)		// Check User/Owner read permission bit
		printf("r");
	else
                printf("-");
	
	if(filePermission.st_mode & S_IWUSR)		// Check User/Owner write permission bit
		printf("w");
	else
                printf("-");
	
	if(filePermission.st_mode & S_IXUSR)		// Check USer/Owner execute permission bit
		printf("x");
	else
                printf("-");

        if(filePermission.st_mode & S_IRGRP)            // Check group read permission bit
                printf("r");
        else
                printf("-");

        if(filePermission.st_mode & S_IWGRP)            // Check group write permission bit
                printf("w");
        else
                printf("-");

        if(filePermission.st_mode & S_IXGRP)            // Check group execute permission bit
                printf("x");
        else
                printf("-");

	if(filePermission.st_mode & S_IROTH)            // Check other read permission bit
                printf("r");
        else
                printf("-");

        if(filePermission.st_mode & S_IWOTH)            // Check other write permission bit
                printf("w");
        else
                printf("-");

        if(filePermission.st_mode & S_IXOTH)            // Check other execute permission bit
                printf("x");
        else
                printf("-");

	printf("\n----- End of Information -----\n");

}


