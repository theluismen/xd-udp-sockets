/* CLIENT (ESQUELET) */

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

int main( int argc, char ** argv ) {

    if ( argc == 3 ) {
        /* Definim variables */
        struct sockaddr_in addr;
        socklen_t addr_size;  // PUEDE QUE NO HAGA NADA
        char buffer[BUFFER_SIZE];
        char * dest_ip;
        int dest_port, sock;

        /* Fem la feina */
        dest_ip   = argv[1];
        dest_port = atoi( argv[2] );
        sock      = socket( AF_INET, SOCK_DGRAM, 0);
        if ( sock >= 0 ) {
            printf("[+] - Socket Creado ( puerto %d)...\n", dest_port);

            memset(&addr, '\0', sizeof(addr) );
            addr.sin_family = AF_INET;
            addr.sin_port   = htons( dest_port );
            addr.sin_addr.s_addr = inet_addr( dest_ip );

            /* Enviar información */
            bzero( buffer, BUFFER_SIZE );
            strcpy( buffer, "Maricones");
            sendto( sock, buffer, BUFFER_SIZE, 0, (struct sockaddr * )&addr, sizeof(addr) );
            printf("[+] - Data Sent: %s\n", buffer);

            bzero( buffer, BUFFER_SIZE );
            addr_size = sizeof(addr);
            recvfrom( sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, &addr_size);
            printf("[+] - Data Recv: %s\n", buffer);

        } else {
            printf("[!] - No se pudo crear el socket.\n");
        }
        /* Tanquem el socket */
        close( sock );
    } else {
        printf("Nombre de paràmetres incorrecte.\n");
        printf("Us apropiat:\n");
        printf("\t./server <ip> <port>\n");
    }

    return 0;
}
