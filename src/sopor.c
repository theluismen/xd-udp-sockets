/* FUNCIONES DE SOPORTE */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void init_addr ( struct sockaddr_in * server_addr, int p, char * ip ) {
    memset(server_addr, '\0', sizeof(server_addr) );
    server_addr->sin_family = AF_INET;
    server_addr->sin_port   = htons( p );
    server_addr->sin_addr.s_addr = inet_addr( ip );
}


void msg_wrong_param ( int n ) {
    printf("[!] - Nombre de paràmetres incorrecte.\n");
    printf("Us apropiat:\n");
    ( n == 0 ) ? printf("\t./server <port>\n") : printf("\t./server <ip> <port>\n");
}
