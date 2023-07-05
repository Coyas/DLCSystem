#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	// 
  	int sockfd, newsockfd, portno, clilen, nread;

	// The server reads characters from the socket connection into this buffer
	char buffer[1024];

	//A sockaddr_in is a structure containing an internet address. This structure is defined in netinet/in.h
	struct sockaddr_in serv_addr, cli_addr;

	//
	 if (argc < 2)
	 {
		 fprintf(stderr,"ERROR, nao foi especificado a porta");
		 exit(1);
	 }

	 sockfd = socket(AF_INET, SOCK_STREAM, 0);

	 if (sockfd < 0)
		 perror("ERROR opening socket");
	
	 // The function bzero() sets all values in a buffer to zero. It takes two arguments, 
	 //the first is a pointer to the buffer and the second is the size of the buffer. Thus, 
	 //this line initializes serv_addr to zeros
	 bzero((char *) &serv_addr, sizeof(serv_addr));

	 // get the numero da porta pelo terminal
	 portno = atoi(argv[1]);
     

	 // The variable serv_addr is a structure of type struct sockaddr_in. 
	 // This structure has four fields. The first field is short sin_family,
	 // which contains a code for the address family. It should always be set to the symbolic constant AF_INET.
	 serv_addr.sin_family = AF_INET;

	 // passar a porta para a estrutura do socket
	 serv_addr.sin_port = htons(portno);

	 // This field contains the IP address of the host.
	 serv_addr.sin_addr.s_addr = INADDR_ANY;

	 // The bind() system call binds a socket to an address, in this case the address of the current host and 
	 // port number on which the server will run.
	 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		 perror("ERROR on binding");

	 // The listen system call allows the process to listen on the socket for connections
	 listen(sockfd,5);

	// it wakes up the process when a connection from a client has been successfully established
	 clilen = sizeof(cli_addr);
	 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	 if (newsockfd < 0)
		 perror("ERROR on accept");

	 // This code initializes the buffer using the bzero() function, and then reads from the socket
	 bzero(buffer,1024);
/*
	 n = read(newsockfd,buffer,255);

	 if (n < 0)
		 perror("ERROR reading from socket");

	 printf("Here is the message: %s",buffer);

	 // Once a connection has been established, both ends can both read and write to the connection.
	 n = write(newsockfd,"I got your message",18);

	 if (n < 0)
		 perror("ERROR writing to socket");
*/
    // Send and receive data.
    while ((nread = read(newsockfd, buffer, sizeof(buffer))) > 0) {
        printf("recebido: %s", buffer);
        write(newsockfd, buffer, nread);
    }

	 return 0;

}
