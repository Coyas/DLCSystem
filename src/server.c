#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, nread;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;

    // Cria um novo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERRO ao abrir o socket");
        exit(1);
    }

    // Limpa a estrutura de endereço do servidor
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Verifica se há argumentos suficientes
    if (argc > 1) {
        // O terceiro argumento é a porta do servidor
        portno = atoi(argv[1]);
        printf("SERVIDOR: listen na porta %d\n", portno);
    } else {
        // Caso contrário, atribui o valor padrão
        fprintf(stderr, "ERRO: Porta não especificada, atribuindo porta padrao 8080\n");
        portno = 8080;
    }

    // Configura a estrutura de endereço do servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Associa o socket ao endereço e porta especificados
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERRO ao realizar o bind");
        exit(1);
    }

    // Inicia a escuta por conexões
    listen(sockfd, 5);

    while (1) {
        clilen = sizeof(cli_addr);

        // Aceita uma nova conexão
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERRO ao aceitar a conexão");
            exit(1);
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("ERRO na criação do processo filho");
            exit(1);
        }

        if (pid == 0) { // Processo filho
            close(sockfd);

            bzero(buffer, 1024);

            // Recebe e envia dados pela conexão
            while ((nread = read(newsockfd, buffer, sizeof(buffer))) > 0) {
                printf("Recebido: %s", buffer);
                //write(newsockfd, buffer, nread);
                write(newsockfd, "dados recebido com sucesso", 27);
                bzero(buffer, 1024);
            }

            if (nread < 0) {
                perror("ERRO ao ler do socket");
                exit(1);
            }

            // Fecha o socket da conexão
            close(newsockfd);
            exit(0);
        } else { // Processo pai
            close(newsockfd);
        }
    }

    // Fecha o socket principal
    close(sockfd);

    return 0;
}
