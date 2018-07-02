// Aaron Liszt
// sortList.c
// lists files in a specified directory with different sorting options

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// Sort lowest to highest (index 0 = lowest)
// compare method used by qsort to compare array elements
int compare(const void * a, const void * b){ return ( *(int*)a - *(int*)b ); }

main(int ac, char *av[]){
	struct stat fileInfo;		// Get file info from each file in the directory
        struct dirent *myfile;		// Pointer to files in mydir
	struct tm *timeInfo;		// Information about the time
	char fileTime[80];		// Store formated time info
	struct dirent **nameList;	// List of filenames
	int numFiles;			// Number of files in the directory
		
	if((ac > 2 && (strcmp(av[2], "-t")) != 0 && (strcmp(av[2], "-s")) != 0) || ac > 3){ // check that av[2] is either -s or -t
                printf("usage: ./sortList[directory] [ [-s] [-t] ]\n", *av);    // arguments must be (./sortList)or(./sortList DIR)or(./sortList DIR -t)or(./sortList DIR -s)
                exit(1);
        }

	if(ac == 1) // If there is one aregument open the current directory an sort by filename
		numFiles = scandir(".", &nameList, 0, alphasort);
	else 	// Otherwise open the specified directory
		numFiles = scandir(av[1], &nameList, 0, alphasort);	

	if(ac == 1 || ac == 2){	// If there are only one/two arguments sort by filename (Already sorted by alphasort)
		numFiles = numFiles - 1;
		while(numFiles >= 0){ // Loop until we have gone through every file
			stat(nameList[numFiles]->d_name, &fileInfo);                        					// Get stat information for current file
                	if((strcmp(nameList[numFiles]->d_name, ".")) != 0 && (strcmp(nameList[numFiles]->d_name, "..")) != 0){	// Ignore . and .. directories
                        	timeInfo = localtime(&fileInfo.st_mtime);       // Give the last modification time of the file to tm structure
                        	strftime(fileTime, 80, "%b %d %R", timeInfo);   // Convert the time into a formatted string
                        	printf("%6d ", fileInfo.st_size);               // Print the size in bytes for each file
                        	printf("%s ", fileTime);                        // Print the last modification date of the file
                        	printf("%s", nameList[numFiles]->d_name);       // Print the filename
                       	 	printf("\n");
			}
			numFiles--;
		}
	}
	else{ // If there are three arguments check the value of av[2] then sort
		numFiles = numFiles - 1; 
		int sortedNums[numFiles];		// Array of sorted times or file sizes (depentds on sort)
		int sortedIndex[numFiles];		// Stores the index of each file in nameList mathing up to the sorted order of sortedNums  	 
		if((strcmp(av[2], "-s")) == 0){ // Sort by file size 
                	int i;  		// loop ints
                	int j;
  
                	for(i = numFiles; i >= 0; --i){ // Loop through nameList and store the size of each file in sortedNums
                        	stat(nameList[i]->d_name, &fileInfo);
                       		sortedNums[i] = fileInfo.st_size;
                	}
                	qsort(sortedNums, numFiles, sizeof(int), compare); // Sort sortedNums

               		for(j = numFiles; j >= 0; --j){// Loop through sortedNums to find the index of each file in nameList and store each index in sortedIndex
				for(i = numFiles; i >= 0; --i){ // Loop through nameList
					stat(nameList[i]->d_name, &fileInfo);
					if(sortedNums[j] == fileInfo.st_size)
						sortedIndex[j] = i;
				}
			}

			for(i = numFiles; i >= 0; --i){  // Loop through nameList once more and use sortedIndex to print out the files and all thier info in sorted order
                                stat(nameList[sortedIndex[i]]->d_name, &fileInfo);    	
                                if((strcmp(nameList[sortedIndex[i]]->d_name, ".")) != 0 && (strcmp(nameList[sortedIndex[i]]->d_name, "..")) != 0){ // Ignore . and .. directories
                                	timeInfo = localtime(&fileInfo.st_mtime);       // Give the last modification time of the file to tm structure
                               		strftime(fileTime, 80, "%b %d %R", timeInfo);   // Convert the time into a formatted string
                                	printf("%6d ", fileInfo.st_size);               // Print the size in bytes for each file
                                	printf("%s ", fileTime);                        // Print the last modification date of the file
                                	printf("%s", nameList[sortedIndex[i]]->d_name); // Print the filename
                                	printf("\n");
                        	}
                        }
        	}
		else{	// If user did not enter -s they must have entered -t
			int a;
			int b;

			for(a = numFiles; a >= 0; --a){	// Loop through nameList and store the raw  modification time of each file.
				stat(nameList[a]->d_name, &fileInfo);
                                sortedNums[a] = fileInfo.st_mtime; 
                        }
			qsort(sortedNums, numFiles, sizeof(int), compare);

                        for(b = numFiles; b >= 0; --b){ // Loop through sortedNums and nameList to find the index in nameList matching the modification time in sortedNums
                                for(a = numFiles; a >= 0; --a){	// and store that index in sortedIndex
                                        stat(nameList[a]->d_name, &fileInfo);
                                        if(sortedNums[b] == fileInfo.st_mtime)
                                                sortedIndex[b] = a;
                                }
                        }

			for(a = numFiles; a >= 0; --a){ // Loop through nameList an use sortedIndex to print out the files in order by modification time
                                stat(nameList[sortedIndex[a]]->d_name, &fileInfo);
                                if((strcmp(nameList[sortedIndex[a]]->d_name, ".")) != 0 && (strcmp(nameList[sortedIndex[a]]->d_name, "..")) != 0){  // Ignore . and .. directories
                                        timeInfo = localtime(&fileInfo.st_mtime);       // Give the last modification time of the file to tm structure
                                        strftime(fileTime, 80, "%b %d %R", timeInfo);   // Convert the time into a formatted string
                                        printf("%6d ", fileInfo.st_size);               // Print the size in bytes for each file
                                        printf("%s ", fileTime);                        // Print the last modification date of the file
                                        printf("%s", nameList[sortedIndex[a]]->d_name); 
                                        printf("\n");
                                }
                        }
		}
	}
}
