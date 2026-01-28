/* server.c */
#include "common.h"

int main() {
	int sockfd, new_sockfd, cli_len;
	struct sockaddr_in serv_addr, cli_addr;
	
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_TCP_PORT);
	serv_addr.sin_addr.s_addr = inet_addr("192.168.0.16"); // Server IP Address Here
	
	printf("Creating socket...\n");
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Server: socket() error\n");
		exit(1);
	}
	
	printf("Binding socket...\n");

	if((bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
		perror("Server: bind() error\n");
		exit(1);
	}

	printf("Listening for connection...\n");
	listen(sockfd, 5);
	
	for(;;) {
		cli_len = sizeof(cli_addr);
		new_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
		
		if(fork() == 0) {
			close(sockfd); // Child no longer listens for new connection
			if(new_sockfd > 0) printf("Client is connected...\n");
			// Continue with the program here
			exit(0);
		}
		
		close(new_sockfd);
	}
	close(sockfd);
}
