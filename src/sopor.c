/* FUNCIONES DE SOPORTE */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "sopor.h"

/* Funciones Usadas por server.c */
int  init_binding ( int sock, struct sockaddr_in * addr) {
    return bind( sock, ( struct sockaddr * ) addr, sizeof(*addr));
}

/* Funciones Usadas por client.c */

/* Funciones Usadas por server.c y client.c */
int  init_sock () {
    return socket( AF_INET, SOCK_DGRAM, 0);
}

void init_addr ( struct sockaddr_in * addr, int p, char * ip ) {
    memset(addr, '\0', sizeof(addr) );
    addr->sin_family = AF_INET;
    addr->sin_port   = htons( p );
    addr->sin_addr.s_addr = inet_addr( ip );
}

void recv_data ( int sock, char * buffer, struct sockaddr_in * src_addr, socklen_t * addr_size ) {
    *addr_size = sizeof(src_addr);
    recvfrom( sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) src_addr, addr_size);
}

void send_data ( int sock, char * buffer, struct sockaddr_in * dest_addr ) {
    sendto( sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) dest_addr, sizeof(*dest_addr));
}

void msg_wrong_param ( int n ) {
    printf("[!] - Nombre de paràmetres incorrecte.\n");
    printf("Us apropiat:\n");
    ( n == 0 ) ? printf("\t./server <port>\n") : printf("\t./server <ip> <port>\n");
}
