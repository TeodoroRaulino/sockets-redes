#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


#define TAMANHO_BUFFER 512          
#define DEFAULT_PORT "3000"        
#define IP_SERVIDOR "127.0.0.1"     // ip do servidor

int main(int argc, char **argv) 
{
    // variaveis
    WSADATA wsaData;                  
    int iResult;                      
    
    SOCKET ConnectSocket = INVALID_SOCKET;    
    
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;    
   	                  
    char sendbuf[TAMANHO_BUFFER];     
    //char recvbuf[TAMANHO_BUFFER];     
    int recvbuflen = TAMANHO_BUFFER;      

    
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    
    char hostname[NI_MAXHOST];        
    gethostname(hostname,NI_MAXHOST); 
    printf("*************\nCliente: Meu nome: %s\n",hostname);

    
    PHOSTENT phe = gethostbyname(hostname);     
    int i;
    for (i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
        printf("Cliente: Meu endereco IP (%d): %s\n",i,inet_ntoa(addr));        
        
    }

    
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;    
    hints.ai_protocol = IPPROTO_TCP;

    
    iResult = getaddrinfo(IP_SERVIDOR, DEFAULT_PORT, &hints, &result);

    
    printf("*************\nTentando se conectar ao servidor de endereco %s na porta %s...\n", IP_SERVIDOR, DEFAULT_PORT);
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Nao foi possivel se conectar ao servidor!\n");
        system("pause");
        WSACleanup();
        return 1;
    }    

    

    i = 0;
    while(!kbhit()) {
    	
        printf("Enviando \"Client %s e envio a mensagem no. %04d\"...\n",hostname,i);                     

        
        sprintf(sendbuf,"Client %s e envio a mensagem no. %04d",hostname,i);
        
        iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );

        i ++;
        Sleep(1000);

    }

    
    iResult = shutdown(ConnectSocket, SD_SEND);

    
    printf("Encerrando socket...");
    closesocket(ConnectSocket);
    WSACleanup();

    system("pause");
    return 0;
}
