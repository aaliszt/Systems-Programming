// Aaron Liszt
// list.c
// Performs all the functions of the linux commmand ls -1

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char permissions[10];

// Returns the permission bits for a given file
const char* permissionBits(struct stat filePermission){
        if(S_ISDIR(filePermission.st_mode))             // Check if we have a directory or a file
        	permissions[0] = 'd';
        else
 		permissions[0] = '-';

        if(filePermission.st_mode & S_IRUSR)            // Check User/Owner read permission bit
                permissions[1] = 'r';
        else
                permissions[1] = '-';

        if(filePermission.st_mode & S_IWUSR)            // Check User/Owner write permission bit
                permissions[2] = 'w';
        else
                permissions[2] = '-';

        if(filePermission.st_mode & S_IXUSR)            // Check User/Owner execute permission bit
                permissions[3] = 'x';
        else
                permissions[3] = '-';

        if(filePermission.st_mode & S_IRGRP)            // Check group read permission bit
                permissions[4] = 'r';
        else
                permissions[4] = '-';

        if(filePermission.st_mode & S_IWGRP)            // Check group write permission bit
                permissions[5] = 'w';
        else
                permissions[5] = '-';

        if(filePermission.st_mode & S_IXGRP)            // Check group execute permission bit
                permissions[6] = 'x';
	else
                permissions[6] = '-';

        if(filePermission.st_mode & S_IROTH)            // Check other read permission bit
                permissions[7] = 'r';
        else
                permissions[7] = '-';

        if(filePermission.st_mode & S_IWOTH)            // Check other write permission bit
                permissions[8] = 'w';
        else
                permissions[8] = '-';

        if(filePermission.st_mode & S_IXOTH)            // Check other execute permission bit
                permissions[9] = 'x';
        else
                permissions[9] = '-';

        return permissions;
}

main(int ac, char *av[]){
	DIR *mydir;			// Pointer to current directory
	struct stat fileInfo;		// Get file info from each file in the directory
        struct dirent *myfile;		// Pointer to files in mydir
	struct passwd *userInfo;	// Information about the user
	struct group *groupInfo;	// Information about the group
	struct tm *timeInfo;		// Information about the time
	char fileTime[80];		// Store formated time info
        int NUM_BLOCKS = 0;		// Number of blocks in mydir

	if ( ac < 1 || ac > 3){		// Check arguments should be either (./list or ./list -l or ./list -l (path))
                printf("usage: ./list (-l) (DIRECTORYPATH)\n", *av);
		exit(1);
	}
	if ((ac == 2 || ac == 3) && (strcmp(av[1], "-l") != 0)){ // "-l must be the 2nd argument
		printf("usage: ./list (-l) (DIRECTORYPATH)\n", *av);
                exit(1);
        }
		
	if(ac == 2 || ac == 1)		// If ac == 2 or 1 open the current directory
		mydir = opendir(".");
	else				// Otherwise open the directory specified in argument 3
		mydir = opendir(av[2]);
	
	if(ac == 1){			// If ac == 1 then only list the files in the directory
                while((myfile = readdir(mydir)) != NULL){       // Loop through the directory and add up the blocks of each file
                	stat(myfile->d_name, &fileInfo);
			if((strcmp(myfile->d_name, ".")) != 0 && (strcmp(myfile->d_name, "..")) != 0)	
                        	printf("%s ", myfile->d_name);  // Print filename
                }
		printf("\n");
                exit(0);
        }
	
	while((myfile = readdir(mydir)) != NULL){ // Loop through the directory and add up the blocks of each file
		stat(myfile->d_name, &fileInfo);
		if((strcmp(myfile->d_name, ".")) != 0 && (strcmp(myfile->d_name, "..")) != 0&& (myfile = readdir(mydir)) != NULL)	// Dont include . and .. directories
			NUM_BLOCKS += fileInfo.st_blocks;	
	}
	printf("total %d \n", NUM_BLOCKS);

	rewinddir(mydir); // Reset pointer to the beggining of the directory	
	while((myfile = readdir(mydir)) != NULL){			// Loop through all the files in the directory
                stat(myfile->d_name, &fileInfo);			// Get stat information for current file
                if(*myfile->d_name != '.'){             		// Ignore . and .. directories
			userInfo = getpwuid(fileInfo.st_uid);		// Get information about the user given their userID
			groupInfo = getgrgid(fileInfo.st_gid);		// Get information about the group given the groupID
			timeInfo = localtime(&fileInfo.st_mtime);	// Give the last modification time of the file to tm structure
			strftime(fileTime, 80, "%b %d %R", timeInfo);	// Convert the time into a formatted string
                        printf("%s ", permissionBits(fileInfo));	// Print out the read/write/execute permissions for each file
			printf("%d ", fileInfo.st_nlink);		// Print out the number of links to the file
			printf("%s ", userInfo->pw_name);		// Print out username
			printf("%s ", groupInfo->gr_name);		// Print out the group name
			printf("%6d ", fileInfo.st_size);		// Print the size in bytes for each file
			printf("%s ", fileTime);			// Print the last modification date of the file
                        printf("%s", myfile->d_name);			// Print the filename
			printf("\n");
                }
        }
	closedir(mydir);
}
