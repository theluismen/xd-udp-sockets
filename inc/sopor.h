/* Constantes para server.c y client.c */
#define SERVER 0
#define CLIENT 1
#define BUFFER_SIZE 1024
#define LOCAL_IP    "127.0.0.1"

/* Prototipos de funciones para server.c y client.c */
void init_addr ( struct sockaddr_in * server_addr, int p, char * ip );
void msg_wrong_param ( int n );
int init_sock ();
int init_binding ( int sock, struct sockaddr_in * addr);
void recv_data ( int sock, char * buffer, struct sockaddr_in * src_addr, socklen_t * addr_size );
void send_data ( int sock, char * buffer, struct sockaddr_in * dest_addr );
