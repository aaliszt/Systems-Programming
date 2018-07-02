// Aaron Liszt 
// client.c 
// Client to communicate with server .c

#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <string.h>
#include <libgen.h>
#include <dirent.h>

int main(int argc,char **argv){
        int sockfd, n, conn;
        struct sockaddr_in servaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
        bzero(&servaddr,sizeof servaddr);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(60007);
        inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
        conn = connect(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr));

	if (conn < 0)
		printf("Connection failed!\n");

        char exit[5] = "quit";		
	char putfile[4] = "put";
        char getfile[4] = "get";
	char endMsg[13] = "STOP READING";	// message to server to stop reading

	FILE* saveFile;		// FILE pointer for saving a file
	FILE* sendFile;		// FILE pointer for sending a file
	char sendBuffer[1024];  // string for sending text
	char recvBuffer[1024];  // string for receiving text
	char sendLine[256];	// string for taking user input

	int bytes_sent;

        while(1){
		printf("Input a client command: ");
                fgets(sendLine, 256, stdin); // read user input
		printf("\n");		

                if(strstr(sendLine, exit) != NULL){ 		     // If client types quit, close client
                        send(sockfd, sendLine, strlen(sendLine), 0); // send message to server to shutdown
                        break;
                }
		if(strstr(sendLine, getfile) != NULL){ // GET A FILE(from the server), save locally under same name
			bytes_sent = send(sockfd, sendLine, strlen(sendLine), 0);
			if(bytes_sent < 0)
				printf("Client error sending command to server!\n");

			char *fileName = sendLine;
			fileName = fileName + 4;  // get rid of "get "

			char *point;			
			if((point = strchr(fileName, '\n')) != NULL)// Remove new line character from input string
					*point = '\0';

			saveFile = fopen(fileName, "w");
			if(saveFile == NULL){
				printf("Client failed to create file!\n");
				continue;
			}
			printf("Client made file\n");
	
			while(((recv(sockfd, recvBuffer, 1024, 0)) > 0) && (strcmp(recvBuffer, "STOP READING") != 0)){ // Recieve from server in a loop, stop on "STOP READING" message
				fputs(recvBuffer, saveFile);
				printf("Client received bytes!\n");
				//printf("%s\n", recvBuffer);
			}

			fclose(saveFile);
			printf("Client received file\n");
                }
                if(strstr(sendLine, putfile) != NULL){ // PUT A FILE(on the server), send files contents to server
			bytes_sent = send(sockfd, sendLine, strlen(sendLine), 0);

			if(bytes_sent < 0)
				printf("Client failed to send comand!\n");

			char *fileName = sendLine;
			fileName = fileName + 4; // remove "put "
			
			char *point;
                        if((point = strchr(fileName, '\n')) != NULL)// Remove new line character from input string
                                        *point = '\0';

			sendFile = fopen(fileName, "r");
			if(sendFile == NULL){
				printf("Client failed to open file!\n");
				continue;
			}
			printf("Client opened file!\n");

			while(fgets(sendBuffer, 1024, sendFile)){	// Send data from file in a loop until we reach EOF
				send(sockfd, sendBuffer, 1024, 0);
				bzero(sendBuffer, 1024);
				printf("Client sent bytes!\n");
				sleep(2);
			}

			printf("Client finished sending!\n");
			send(sockfd, endMsg, 13, 0);			// Tell server to stop reading
			fclose(sendFile);
                }
        }
	int i;
        i = close(sockfd);
	if(i < 0)
		printf("Client close() failed\n");

        printf("Client exited\n");
}
