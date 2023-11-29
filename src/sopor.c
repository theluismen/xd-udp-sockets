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

int registrar ( struct Pdu * dgram ) {
    FILE * users = fopen(REGIST_FILENAME, "a"); // ab -> Añadir (append) lineas en binario
    int err = fprintf(users, "%s %s %s\n", dgram->username, dgram->nickname, dgram->md5hexpaswd );
    // int err = fwrite( buffer, users);
    fclose(users);
    return err;
}

int existe_usuario ( struct Pdu * dgram ) {
    FILE * users = fopen(REGIST_FILENAME, "r"); // r -> Leer
    char username_aux[USERNAME_SIZE+1];
    int existe = 0;

    if ( users != NULL ) {
        while ( ! existe && ! feof(users) ) {
            fscanf(users, "%s %*s %*s\n", username_aux);
            if ( strcmp(dgram->username, username_aux) == 0 ) {
                existe = 1;
            }
        }
    }
    return existe;
}

int esta_registrado ( struct Pdu * dgram ) {
    FILE * users = fopen(REGIST_FILENAME, "r"); // r -> Leer
    char username_aux[USERNAME_SIZE+1];
    char md5paswd_aux[MD5_SIZE*2];
    int esta = 0;

    if ( users != NULL ) {
        while ( ! esta && ! feof(users) ) {
            fscanf(users, "%s %s %s\n", username_aux, dgram->nickname, md5paswd_aux);
            if ( strcmp(dgram->username, username_aux) == 0 ) {
                if ( strcmp(dgram->md5hexpaswd, md5paswd_aux) == 0 ) {
                    esta = 2;
                } else {
                    esta = 1;
                }
            }
        }
    }
    return esta;
}

/* Funciones Usadas por client.c */
void decode_str ( char * str ) {
    int i;
    for ( i = 0; str[i] != '\0'; i++ ) {
        if ( str[i] == '%' ) str[i] = '\n';
        if ( str[i] == '_' ) str[i] = ' ';
    }
}

void encode_str ( char * str ) {
    int i;
    for ( i = 0; str[i] != '\0' && str[i] != '\n'; i++ ) {
        if ( str[i] == ' ' ) str[i] = '_';
    }
}

void limpiar_entrada ( ) {
    while ( getchar() != '\n' ) {
        printf("1" );
    }
}

void pedir_nickname ( struct Pdu * dgram ) {
    printf("  Nickname: ");
    fgets(dgram->nickname, NICKNAME_SIZE+1, stdin); // Guardar los 15 caracteres (puede tener espacios)
    limpiar_entrada();                              // Eliminar del buffer loque haya
    encode_str(dgram->nickname);                    // codificar espacios
}

void pedir_username ( struct Pdu * dgram ) {
    printf("  Username: ");
    scanf("%15s", dgram->username );             // Guardar los 15 caracteres
    limpiar_entrada();                           // Eliminar del buffer loque haya
}

void pedir_passwdx2 ( struct Pdu * dgram ) {
    char password [PASSWORD_SIZE+1];
    char password2[PASSWORD_SIZE+1];
    uint8_t md5paswd[MD5_SIZE];
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

    md5String(password, md5paswd);     // Calcular md5 en el dgram
    to_md5_hex(md5paswd, MD5_SIZE, dgram->md5hexpaswd);
}

void pedir_password ( struct Pdu * dgram ) {
    char password[PASSWORD_SIZE+1];
    uint8_t md5paswd[MD5_SIZE];
    printf("  Password: ");
    scanf("%15s", password );             // Guardar los 15 caracteres
    limpiar_entrada();
    md5String(password, md5paswd);        // Calcular md5 en el dgram
    to_md5_hex(md5paswd, MD5_SIZE, dgram->md5hexpaswd);
}

void to_md5_hex ( char * str, int str_size, char * md5hex ) {
    int i;
    unsigned char n, m;
    for ( i = 0; i < str_size; i++ ) {
        n = ( str[i] & 0b11110000 ) >> 4;
        m = str[i] & 0b00001111;
        md5hex += sprintf(md5hex, "%x%x", n, m);
    }
}

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
