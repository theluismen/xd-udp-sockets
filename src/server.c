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

/* Inclusio de fitxers .h propis del projecte */
#include "sopor.h"

int main ( int argc, char ** argv ) {

    /* Comprobar que se tiene el puerto de escucha */
    if ( argc != 2 ) {
        msg_wrong_param( SERVER );
        return 0;
    }

    /* Definim variables */
    struct sockaddr_in server_addr, client_addr;
    struct Pdu dgram;
    socklen_t  addr_size;
    char buffer[BUFFER_SIZE];
    int port, sock, bind;

    /* Guardamos el puerto y creamos el socket */
    port = atoi( argv[1] );
    sock      = init_sock();
    if ( sock < 0 ) {
        printf("[!] - No se pudo crear el socket.\n");
        return 0;
    }

    /* Si el socket se crea correctamente... */
    printf("[+] - Socket creado ( :%d )...\n", port);
    init_addr( &server_addr, port, LOCAL_IP);
    bind = init_binding( sock, &server_addr );
    if ( bind < 0 ) {
        printf("[!] - No se pudo realizar el bind().\n");
        return 0;
    }

    /* Bucle Principal del Servidor */
    printf("[+] - Binded...\n");
    while ( 1 ) {
        /* Recibir datos */
        bzero( buffer, BUFFER_SIZE );
        recv_data( sock, buffer, &client_addr, &addr_size);
        printf("[+] - Data Recv: %s\n", buffer);

        /* Procesar datos */
        sscanf( buffer, "%hd", &dgram.flag );
        switch ( dgram.flag ) {
            case PDU_FLAG_WELCOME_ASK:
                sprintf( buffer, "%hd %s", PDU_FLAG_WELCOME_MSG, WELCOME_MSG);
                break;;
            case PDU_FLAG_REGIST_ASK:
                sscanf( buffer, "%*d %15s %15s %16s", dgram.username, dgram.nickname, dgram.md5paswd );
                dgram.flag = PDU_FLAG_REGIST_OK;
                if ( ! esta_registrado( &dgram ) ) {
                    if ( registrar( &dgram ) < 0 ) {
                        dgram.flag = PDU_FLAG_REGIST_ERR;
                    }
                } else {
                    dgram.flag = PDU_FLAG_REGIST_DUP;
                }
                sprintf( buffer, "%hd", dgram.flag);
                break;;
        }

        /* Enviar datos de vuelta */
        send_data( sock, buffer, &client_addr );
        printf("[+] - Data Sent: %s\n", buffer);
    }

    /* Tanquem el socket */
    close( sock );

    return 0;
}














//
