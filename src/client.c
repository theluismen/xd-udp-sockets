/* CLIENT (ESQUELET) */

/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Inclusio de fitxers .h propis del projecte */
#include "sopor.h"
#include "md5.h"

int main( int argc, char ** argv ) {

    /* Comprobar que se tiene el <ip>:<port> */
    if ( argc != 3 ) {
        msg_wrong_param( CLIENT );
        return 0;
    }

    /* Definim variables */
    struct sockaddr_in addr;
    struct Pdu dgram;
    socklen_t addr_size = sizeof(addr);
    char buffer[BUFFER_SIZE], msg[BUFFER_SIZE];
    char * dest_ip;
    int dest_port, sock;
    int opt;
    bool bye = false;

    /* Guardar ip y puerto y crear socket */
    dest_ip   = argv[1];
    dest_port = atoi( argv[2] );
    sock      = init_sock();
    if ( sock < 0 ) {
        printf("[!] - No se pudo crear el socket.\n");
        return 0;
    }

    /* Si el socket se crea correctamente... */
    init_addr( &addr, dest_port, dest_ip);
    bzero( buffer, BUFFER_SIZE );
    bzero( msg, BUFFER_SIZE );

    /* Mensaje de bienvenida */
    sprintf( buffer, "%d", PDU_FLAG_WELCOME_ASK );  // Preparar buffer
    send_data( sock, buffer, &addr);                // Enviar   información
    recv_data( sock, buffer, &addr, &addr_size);    // Recibir  datos
    sscanf( buffer, "%hd %s", &dgram.flag, msg );   // Parsear  buffer

    if ( dgram.flag == PDU_FLAG_WELCOME_MSG ) {
        sscanf( buffer, "%*d %s", msg);
        decode_str( msg );
        printf("%s", msg );

        do {
            printf(" > ");
            scanf("%d", &opt);
            switch ( opt ) {
                case OPT_REG:
                    /* Pedir Info */
                    pedir_username( &dgram );
                    pedir_nickname( &dgram );
                    pedir_passwdx2( &dgram );
                    /* Preparar envio de datos y enviar */
                    sprintf( buffer, "%d %s %s %s", PDU_FLAG_REGIST_ASK, dgram.username, dgram.nickname, dgram.md5hexpaswd); // Preparar buffer
                    send_data( sock, buffer, &addr);             // Enviar información
                    /* Recibir confirmacion de registro */
                    recv_data( sock, buffer, &addr, &addr_size);
                    sscanf( buffer, "%hd", &dgram.flag );
                    /* Mostrar resultado por pantalla */
                    switch ( dgram.flag ) {
                        case PDU_FLAG_REGIST_OK:
                            printf("  Registro completado\n");
                            break;;
                        case PDU_FLAG_REGIST_ERR:
                            printf("  Registro NO Completado. Ocurrieron errores\n");
                            break;;
                        case PDU_FLAG_REGIST_DUP:
                            printf("  Registro NO Completado. Username repetido\n");
                            break;;
                    }
                    break;;
                case OPT_INI:
                    /* Pedir Info */
                    pedir_username( &dgram );
                    pedir_password( &dgram );
                    /* Preparar envio de datos y enviar */
                    sprintf( buffer, "%d %s %s", PDU_FLAG_INITSE_ASK, dgram.username, dgram.md5hexpaswd); // Preparar buffer
                    send_data( sock, buffer, &addr);             // Enviar información
                    /* Recibir confirmacion de inicio de sesion */
                    recv_data( sock, buffer, &addr, &addr_size);
                    sscanf( buffer, "%hd", &dgram.flag );
                    /* Mostrar resultado por pantalla */
                    switch ( dgram.flag ) {
                        case PDU_FLAG_INITSE_ERR: // No existe usuario
                            printf("  Usuario desconocido\n");
                            break;;
                        case PDU_FLAG_INITSE_MID: // Existe usuario, contra incorrecta
                            printf("  Contraseña incorrecta\n");
                            break;;
                        case PDU_FLAG_INITSE_OK: // Existe usuario, contra correcta
                            sscanf( buffer, "%*d %s", dgram.nickname );
                            decode_str(dgram.nickname);
                            printf("  Hola %s !\n", dgram.nickname);
                            break;;
                    }
                break;;
                case OPT_BYE:
                    bye = true;
                break;;
                default:
                    printf("  Opcio invalida\n");
            }
        } while ( opt < 0 || opt > 2 || ! bye );
    }

    /* Tanquem el socket */
    close( sock );

    return 0;
}
