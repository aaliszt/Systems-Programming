// Aaron Liszt
// server.c
// Server to recive simple commands and either save or send files

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>

int main(){
        int sock, i, sock_recv, addr_size;
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Initialize Socket

        struct sockaddr_in server_addr; // Server address struct

        if(sock < 0) 			// Check socket
                printf("socket() failed\n");

        memset(&server_addr, 0, sizeof(server_addr));   // Set all of server_addr to 0
        server_addr.sin_family = AF_INET;               // Set address family
        server_addr.sin_addr.s_addr = htons(INADDR_ANY);// Socket accepts from local IP
        server_addr.sin_port = htons(60007);  		// Listen on port

        i = bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));  // Bind socket to local IP
        if(i < 0) // Check bind
                printf("Bind failed\n");

        i = listen(sock, 5);    // Listen for connections
        if (i < 0) 		// Check listen
                printf("listen() failed\n");

        sock_recv = accept(sock, (struct sockaddr *) NULL, NULL); // Accept client connections

        char text[256];			   // Read in text from client
	char sendBuff[1024];		   // buffer for receiving text
	char recvBuff[1024];		   // buffer for sending text
        char exit[5] = "quit";		   // Look for quit in received lines to exit server
	char putfile[4] = "put";	   // Look for put to put files
	char getfile[4] = "get";	   // Look for get to get files
	char endSend[13] = "STOP READING"; // send to client to stop recving
	
	FILE *saveFile;		// file pointer for saving files
	FILE *sendFile;		// file pointer for sending files

	int bytes_received;

        while(1){
                bytes_received = recv(sock_recv, text, 256, 0);  // Recieve message from client

                if(strstr(text, exit) != NULL){  // If we recive "shutdown" message close the server
                        break;
		}
		else if(strstr(text, getfile) != NULL){	// SEND A FILE
			char *fileName = text;
			fileName = fileName + 4;  // get rid of "get "

			char *point;			
			if((point = strchr(fileName, '\n')) != NULL)// Remove new line character from filename
					*point = '\0';
			
			sendFile = fopen(fileName, "r"); // Open file in read only mode
			if(sendFile == NULL){
				printf("Server failed to open file!\n");
				continue;
			}
			printf("Server opened file!\n");
			while(fgets(sendBuff, 1024, sendFile)){		// Send bytes in a loop until EOF
				send(sock_recv, sendBuff, 1024, 0);
				bzero(sendBuff, 1024);
				printf("Server sent bytes!\n");
				sleep(2);
			}
			printf("Server finished sending!\n");
			send(sock_recv, endSend, 13, 0);	// Send stop reading message to client to exit loop
			fclose(sendFile);		
		}
		else if(strstr(text, putfile) != NULL){ // SAVE A FILE
			char *fileName = text;
			fileName = fileName + 4;  // get rid of "get "

			char *point;			
			if((point = strchr(fileName, '\n')) != NULL)// Remove new line character from filename
					*point = '\0';

			saveFile = fopen(fileName, "w");
			if(saveFile == NULL){
				printf("Server failed to create file!\n");
				continue;
			}
			printf("Server created file!\n");
			
			while(((recv(sock_recv, recvBuff, 1024, 0)) > 0) && (strcmp(recvBuff, "STOP READING") != 0)){ // recieve until client sends "STOP READING" message
				fputs(recvBuff, saveFile);
				printf("Server received bytes!\n");
				//printf("%s\n", recvBuffer);
				bzero(recvBuff, 1024);
			}
			fclose(saveFile);
			printf("Server received File!\n");
		}
		else // Echo back to client (This will never happpen because my cleint will ignore anything that is not a valid command
			 printf("Recieved: %s\n", text);  // Echo back whatever the client sent

		bzero(text, 256);
	}
	i = close(sock_recv); // close the socket
        if(i < 0)
                printf(" Server close() failed\n");

        printf("Server shutdown successfull\n");
}

