# DLCSystem

## Datalink Comunication Systems

Este systema propem uma nova abordagem do uso dos sistemas de chat, baseado principalmente no sistema de comunicaçao 'irc' (internet relay chat)# DLCSystem

## Compilaçao

Para compilar o projeto dev ser usado o makefile. Mas tambem pode-se fazer usando o modo padrao.

O sistema sincrono permite conectar e enviar mensagens continuamente ate que o utilizador termine a conexao.

O sistema Assincrono permite conectar, enviar uma mensagem e sair (terminar a execuçao do cliente).


```
Systema Assyncrono:
gcc src/cliente.c -o bin/cliente
gcc src/server.c -o bin/server

Systema Syncrono (*buggy):
gcc src/cliente_syncrono.c -o bin/cliente_syncrono
gcc src/server_syncrono.c -o bin/server_syncrono
```


## Cliente

como usar:

cliente                    // Usará o IP e a porta padrão

cliente 8080               // Usará o IP padrão e a porta fornecida

cliente 192.168.0.1        // Usará o IP fornecido e a porta padrão (8080) * esta com erro ainda

cliente 192.168.0.1 8080   // Usará o IP e a porta fornecidos

## Servidor

como usar:

server                    // Usará o IP da maquina e a porta padrão

server 8080               // Usará o IP da maquina e a porta fornecida