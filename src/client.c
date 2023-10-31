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

/* Inclusio de fitxers .h propis del projecte */
#include "sopor.h"

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
        sock      = init_sock();
        if ( sock >= 0 ) {
            printf("[+] - Socket Creado ( puerto %d)...\n", dest_port);
            init_addr( &addr, dest_port, dest_ip);

            /* Enviar información */
            bzero( buffer, BUFFER_SIZE );
            strcpy( buffer, "Maricones");
            send_data( sock, buffer, &addr);
            printf("[+] - Data Sent: %s\n", buffer);

            /* Recibir datos */
            bzero( buffer, BUFFER_SIZE );
            addr_size = sizeof(addr);
            recv_data( sock, buffer, &addr, &addr_size);
            printf("[+] - Data Recv: %s\n", buffer);

        } else {
            printf("[!] - No se pudo crear el socket.\n");
        }
        /* Tanquem el socket */
        close( sock ); // Tanquem el socket
    } else {
        msg_wrong_param( CLIENT );
    }

    return 0;
}
