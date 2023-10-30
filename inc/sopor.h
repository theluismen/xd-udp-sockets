#define SERVER 0
#define CLIENT 1
#define BUFFER_SIZE 1024
#define LOCAL_IP    "127.0.0.1"
void init_addr ( struct sockaddr_in * server_addr, int p, char * ip );
void msg_wrong_param ( int n );
