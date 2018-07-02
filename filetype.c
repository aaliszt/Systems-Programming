// Aaron Liszt
// filetype.c
// print the type of a file given the filename

#include <sys/stat.h>
#include <stdio.h>

main(int ac, char *av[]){

	if ( ac != 2 ){
                printf("usage: %s filename \n", *av);
        }

	struct stat fileType;

	stat(av[1], &fileType);
	
	printf("\n----- Information about %s -----\n", av[1]);

	if(S_ISREG(fileType.st_mode))					// Check if path is Regular file
		printf("%s is a Regular file\n", av[1]);
	else
		printf("%s is not a Regular file\n", av[1]);

	if(S_ISDIR(fileType.st_mode))					// Check if path is a Directory
		printf("%s is a Directory file\n", av[1]);
	else
		printf("%s is not a Directory file\n", av[1]);

	if(S_ISCHR(fileType.st_mode))					// Check if path is a Character special file
                printf("%s is a Character special file\n", av[1]);
	else
		printf("%s is not a Character special file\n", av[1]);

	if(S_ISBLK(fileType.st_mode))					// Check if path is a Block special file
                printf("%s is a Block special file\n", av[1]);
	else
		printf("%s is not a Block special file\n", av[1]);

	if(S_ISFIFO(fileType.st_mode))					// Check if path is a pip or FIFO
                printf("%s is a pipe or FIFO\n", av[1]);
	else
		printf("%s is not a pipe or FIFO\n", av[1]);

	if(S_ISLNK(fileType.st_mode))					// Check if path is a Sybolic link
                printf("%s is a Symbolic link\n", av[1]);
	else
		printf("%s is not a Symbolic link\n", av[1]);

	if(S_ISSOCK(fileType.st_mode))					// Check if path is a Socket
                printf("%s is a Socket\n", av[1]);
	else
		printf("%s is not a Socket\n", av[1]);

	printf("----- End of Information -----\n");
}
