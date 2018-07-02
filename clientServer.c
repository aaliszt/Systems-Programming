// Aaron Liszt
// clientServer.c
// Simple client server interaction with a socket

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(){
	int sock, i, sock_recv, addr_size;
	sock = socket(AF_INET, SOCK_STREAM, 0); // Initialize Socket

	struct sockaddr_in server_addr;         // Server address struct

	if(sock < 0) // Check socket
		printf("socket() failed\n");

	memset(&server_addr, 0, sizeof(server_addr));	// Set all of server_addr to 0
	server_addr.sin_family = AF_INET;		// Set address family
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);// Socket accepts from local IP
	server_addr.sin_port = htons(60000);		// Listen on port 60000

	i = bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));	// Bind socket to local IP
	if(i < 0) // Check bind
		printf("Bind failed\n");

	i = listen(sock, 5);	// Listen for connections
	if (i < 0) // Check listen
		printf("listen() failed\n");

	sock_recv = accept(sock, (struct sockaddr *) NULL, NULL); // Accept client connections

	int bytes_received;
	char text[80];
	char exit[9] = "shutdown";
	
	while(1){
		bytes_received = recv(sock_recv, text, 80, 0);	// Revieve message from client
		text[bytes_received] = 0;
		
		printf("Recieved: %s\n",text);	// Echo back whatever the client sent

		if(strstr(text, exit) != NULL)  // If we recive "shutdown" message close the server
			break;
		
		//write(sock_recv, text, strlen(text)+1); // return message to client (unnessesary)
	}
	i = close(sock_recv); // close the socket
	if(i < 0)
		printf("close() failed\n");

	printf("Server shutdown successfull\n");
}