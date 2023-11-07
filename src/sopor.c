/* FUNCIONES DE SOPORTE */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "sopor.h"
#include "md5.h"

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

void decode_str ( char * str ) {
    int i;
    for ( i = 0; str[i] != '\0'; i++ ) {
        if ( str[i] == '%' ) str[i] = '\n';
        if ( str[i] == '_' ) str[i] = ' ';
    }
}

void limpiar_entrada ( ) {
    while ( getchar() != '\n' ) {}
}

void pedir_nickname ( struct Pdu * dgram ) {
    printf("  Nickname: ");
    scanf("%15s", dgram->nickname );             // Guardar los 15 caracteres
    limpiar_entrada();                           // Eliminar del buffer loque haya
}

void pedir_username ( struct Pdu * dgram ) {
    printf("  Username: ");
    scanf("%15s", dgram->username );             // Guardar los 15 caracteres
    limpiar_entrada();                           // Eliminar del buffer loque haya
}

void pedir_password ( struct Pdu * dgram ) {
    char password[PASSWORD_SIZE+1], password2[PASSWORD_SIZE+1];
    int cmp;
    do {
        printf("  Password: ");
        scanf("%15s", password );             // Guardar los 15 caracteres
        limpiar_entrada();                    // Eliminar del buffer loque haya
        printf("  Confirma: ");
        scanf("%15s", password2 );            // Guardar los 15 caracteres
        limpiar_entrada();                    // Eliminar del buffer loque haya
        cmp = strcmp(password, password2);
        if ( cmp != 0 ) {
            printf("  Las contraseñas no coinciden: \n");
        }
    } while ( cmp != 0 );

    md5String(password, dgram->md5paswd);
}
