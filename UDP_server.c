// Aaron Liszt
// UDP_server.c
// A simple conectionless (UDP) server

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

main(){
	int sock, i;
	sock = socket(AF_INET, SOCK_DGRAM, 0); // Initialize Socket

	struct sockaddr_in server_addr;         // Server address struct

	if(sock < 0){ // Check socket
		printf("socket() failed\n");
		exit(1);
	}
	memset(&server_addr, 0, sizeof(server_addr));	// Set all of server_addr to 0
	server_addr.sin_family = AF_INET;		// Set address family
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);// Socket accepts from local IP
	server_addr.sin_port = htons(60000);		// Listen on port 60000

	i = bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));	// Bind socket to local IP
	if(i < 0){ // Check bind
		printf("Bind failed\n");	
		exit(2);
	}

	int bytes_received;
	char text[256];
	char end[9] = "shutdown";

	while(1){
		bytes_received = recvfrom(sock, text, 256, 0, NULL, 0);	// Get message from clients
		text[bytes_received] = 0;
		
		printf("Recieved: %s\n", text);	// Echo clients message

		if(strstr(text, end) != NULL)  // If we recive "shutdown" message close the server
			break;
	}
	i = close(sock); // close the socket
	if(i < 0){
		printf("close() failed\n");
		exit(3);
	}

	printf("Server shutdown successfull\n");
}
