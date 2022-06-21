#include <winsock2.h>                //sockets
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define TAMANHO_BUFFER 512          //buffer
#define DEFAULT_PORT "3000"        //porta

int __cdecl main(void) 
{
    // variáveis
	WSADATA wsaData;                                                     
    int iResult;       
    
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[TAMANHO_BUFFER];
    int recvbuflen = TAMANHO_BUFFER; 
      

    // inicializa o winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    
    //Nome/Ip
    char hostname[NI_MAXHOST];        
    gethostname(hostname,NI_MAXHOST);
    printf("*************\nServidor: Meu nome: %s\n",hostname);

    
    PHOSTENT phe = gethostbyname(hostname);     
    int i;
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Servidor: Meu endereco IP (%d): %s\n",i, inet_ntoa(addr));        
        
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve o endereco do servidor e a porta 
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    // Cria um socket para conexao com o servidor
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // Configura o socket TCP do servidor
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);


    freeaddrinfo(result);

    // Escuta aguardando um cliente 
    printf("*************\nServidor: Estou aguardando um cliente na porta %s...\n", DEFAULT_PORT);
    iResult = listen(ListenSocket, SOMAXCONN);

    // Aceita o socket com o client
    ClientSocket = accept(ListenSocket, NULL, NULL);


    // Fecha o ListenSocket por já ter um client conectado
    closesocket(ListenSocket);

    // conexão com o client
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Mensagem recebida: \"%s\"\n", recvbuf);

        }
        else if (iResult == 0)
            printf("Fechando conexao...\n");
        else  {
            printf("recv falhou com o erro: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            system("pause");
            return 1;
        }

    } while (iResult > 0);

    // Encerra a conexao
    iResult = shutdown(ClientSocket, SD_SEND);
    

    // cleanup
    printf("Encerrando socket...");
    closesocket(ClientSocket);
    WSACleanup();

    system("pause");
    return 0;
}
