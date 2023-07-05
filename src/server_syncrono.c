#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CLIENTS_MAX 2

typedef struct {
    int client_socket;
    char client_id[32];
    char message[BUFFER_SIZE];
} ClientInfo;

void printClientInfo(const ClientInfo* client) {
    printf("ID do cliente: %s\n", client->client_id);
    printf("Socket do cliente: %d\n", client->client_socket);
    printf("Mensagem do cliente: %s\n", client->message);
}
void handle_client(int client_socket, ClientInfo* client_list, int* num_clients) {
    char buffer[BUFFER_SIZE];
    ssize_t nread;

    // Recebe o ID do cliente
    nread = read(client_socket, buffer, sizeof(buffer));
    if (nread < 0) {
        perror("ERRO ao ler do socket");
        return;
    }
    buffer[nread] = '\0';

    // Verifica se há espaço para mais clientes
    if (*num_clients < 2) {
        // Inicializa a estrutura do cliente
        ClientInfo* client = &client_list[*num_clients];
        client->client_socket = client_socket;
        strcpy(client->client_id, buffer);
        memset(client->message, 0, sizeof(client->message)); // Inicializa a mensagem com zeros
        (*num_clients)++;

        // Imprime as informações do cliente
        printf("ID do cliente: %s\n", client->client_id);
        printf("Socket do cliente: %d\n", client->client_socket);
        printf("Mensagem do cliente: %s\n", client->message);
    } else {
        printf("Número máximo de clientes atingido. Conexão rejeitada.\n");
        return;
    }

    // Aqui você pode adicionar a lógica para lidar com as mensagens do cliente

    // Exemplo: envia uma mensagem de confirmação ao cliente
    char message[] = "Dados recebidos com sucesso";
    write(client_socket, message, sizeof(message));
}


void handle_client2(int client_socket, ClientInfo* client_list, int* num_clients) {
    char buffer[BUFFER_SIZE];
    ssize_t nread;

    // Recebe o ID do cliente
    nread = read(client_socket, buffer, sizeof(buffer));
    if (nread < 0) {
        perror("ERRO ao ler do socket");
        return;
    }
    buffer[nread] = '\0';

    // Armazena o ID do cliente na estrutura de dados
    if (*num_clients < CLIENTS_MAX) {
        ClientInfo* client = &client_list[*num_clients];
        client->client_socket = client_socket;
        strcpy(client->client_id, buffer);
        strcpy(client->message, buffer);
        (*num_clients)++;
        printf("Cliente conectado: ID=%s\n", client->client_id);
    } else {
        printf("Número máximo de clientes atingido. Conexão rejeitada.\n");
        return;
    }

    // Aqui você pode adicionar a lógica para lidar com as mensagens do cliente

    // Exemplo: envia uma mensagem de confirmação ao cliente
    char message[] = "Dados recebidos com sucesso";
    write(client_socket, message, sizeof(message));
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    struct sockaddr_in serv_addr, cli_addr;
    ClientInfo client_list[CLIENTS_MAX];
    int num_clients = 0;

    // Cria um novo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERRO ao abrir o socket");
        exit(1);
    }

    // Limpa a estrutura de endereço do servidor
    bzero((char *)&serv_addr, sizeof(serv_addr));

    // Verifica se há argumentos suficientes
    if (argc > 1) {
        // O terceiro argumento é a porta do servidor
        portno = atoi(argv[1]);
        printf("SERVIDOR: listen na porta %d\n", portno);
    } else {
        // Caso contrário, atribui o valor padrão
        fprintf(stderr, "ERRO: Porta não especificada, atribuindo porta padrão 8080\n");
        portno = 8080;
    }

    // Configura a estrutura de endereço do servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Associa o socket ao endereço e porta especificados
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERRO ao realizar o bind");
        exit(1);
    }

    // Inicia a escuta por conexões
    listen(sockfd, 5);

    //printClientInfo(&client_list);
    while (1) {
        clilen = sizeof(cli_addr);

        // Aceita uma nova conexão
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
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

            // Lida com a conexão do cliente
            handle_client(newsockfd, client_list, &num_clients);

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
