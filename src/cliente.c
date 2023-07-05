#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

//#define SERVER_IP "127.0.0.1"
//#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_IP_LENGTH 16

int main(int argc, char *argv[]) {
    int sockfd, PORT;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE], SERVER_IP[MAX_IP_LENGTH];;

    if (argc < 2)
	{
	 fprintf(stderr,"ERROR, nao foi especificado a porta");
	 exit(1);
	}

    
    // Verifica se há argumentos suficientes
    if (argc >= 2) {
        // O segundo argumento é o IP do servidor
        strcpy(SERVER_IP, argv[1]);
    } else {
        // Caso contrário, atribui o valor padrão
        strcpy(SERVER_IP, "127.0.0.1");
    }

    // Verifica se há argumentos suficientes
    if (argc >= 3) {
        // O terceiro argumento é a porta do servidor
        PORT = atoi(argv[2]);
    } else {
        // Caso contrário, atribui o valor padrão
        PORT = 8080;
    }
	

    // Cria um socket TCP (SOCK_STREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    // Configura o endereço do servidor
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    // Estabelece a conexão com o servidor
    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Erro ao estabelecer a conexão com o servidor");
        exit(EXIT_FAILURE);
    }

    printf("Conexão estabelecida com o servidor.\n");

    // Envia dados para o servidor
    printf("Digite uma mensagem para enviar ao servidor: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    ssize_t num_bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
    if (num_bytes_sent < 0) {
        perror("Erro ao enviar dados para o servidor");
        exit(EXIT_FAILURE);
    }

    // Recebe a resposta do servidor
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t num_bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (num_bytes_received < 0) {
        perror("Erro ao receber dados do servidor");
        exit(EXIT_FAILURE);
    }

    printf("Resposta do servidor: %s\n", buffer);

    // Fecha o socket
    close(sockfd);

    return 0;
}