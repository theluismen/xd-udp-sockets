/* SERVIDOR (ESQUELET) */

/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define LOCAL_IP    "127.0.0.1"

int main ( int argc, char ** argv ) {

    if ( argc == 2 ) {
        /* Definim variables */
        struct sockaddr_in server_addr, client_addr;
        socklen_t addr_size;
        char buffer[BUFFER_SIZE];
        int bind_port, sock, binded;

        /* Fem la feina */
        bind_port = atoi( argv[1] );
        sock      = socket( AF_INET, SOCK_DGRAM, 0);
        if ( sock >= 0 ) {
            printf("[+] - Socket Creado ( puerto %d)...\n", bind_port);

            memset(&server_addr, '\0', sizeof(server_addr) );
            server_addr.sin_family = AF_INET;
            server_addr.sin_port   = htons( bind_port );
            server_addr.sin_addr.s_addr = inet_addr( LOCAL_IP );

            binded = bind( sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
            if ( binded >= 0 ) {
                printf("[+] - Binded...\n");

                /* Recibir datos */
                bzero( buffer, BUFFER_SIZE );
                addr_size = sizeof(client_addr);
                recvfrom( sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, &addr_size);
                printf("[+] - Data Recv: %s\n", buffer);

                /* Enviar datos de vuelta */
                bzero( buffer, BUFFER_SIZE );
                strcpy( buffer, "Tu puta madre jaja");
                sendto( sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
                printf("[+] - Data Sent: %s\n", buffer);
            } else {
                printf("[!] - No se pudo realizar el bind().\n");
            }
        } else {
            printf("[!] - No se pudo crear el socket.\n");
        }
        /* Tanquem el socket */
        close( sock );
    } else {
        printf("[!] - Nombre de paràmetres incorrecte.\n");
        printf("Us apropiat:\n");
        printf("\t./server <port>\n");
    }

    return 0;
}














//
