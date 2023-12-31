/* Constantes para server.c y client.c */
#define SERVER 0
#define CLIENT 1
#define BUFFER_SIZE 1024
#define LOCAL_IP    "127.0.0.1"

#define REGIST_FILENAME "./data/users.txt"

#define PDU_FLAG_WELCOME_ASK 0
#define PDU_FLAG_WELCOME_MSG 1

#define PDU_FLAG_REGIST_ASK  2
#define PDU_FLAG_REGIST_OK   3
#define PDU_FLAG_REGIST_ERR  4
#define PDU_FLAG_REGIST_DUP  5

#define PDU_FLAG_INITSE_ASK  6
#define PDU_FLAG_INITSE_OK   7
#define PDU_FLAG_INITSE_ERR  8
#define PDU_FLAG_INITSE_MID  9

#define OPT_REG 1
#define OPT_INI 2
#define OPT_BYE 0

#define WELCOME_MSG "###################%#_Benvingut_!!_<3_#%###################%__Que_vols_fer_?%_1_-_Registrarse%_2_-_Iniciar_Sesio%_0_-_Sortir%"

/* Estructuras */
#define NICKNAME_SIZE 15
#define USERNAME_SIZE 15
#define PASSWORD_SIZE 15
#define MD5_SIZE      16 //128bits

struct Pdu {
    short int flag;
    char username[USERNAME_SIZE+1];
    char nickname[NICKNAME_SIZE+1];
    unsigned char md5hexpaswd[MD5_SIZE*2];
};

/* Prototipos de funciones para server.c y client.c */
void init_addr ( struct sockaddr_in * server_addr, int p, char * ip );
void msg_wrong_param ( int n );
int init_sock ();
int init_binding ( int sock, struct sockaddr_in * addr);
void recv_data ( int sock, char * buffer, struct sockaddr_in * src_addr, socklen_t * addr_size );
void send_data ( int sock, char * buffer, struct sockaddr_in * dest_addr );
void decode_str ( char * str );
void pedir_nickname ( struct Pdu * dgram );
void pedir_nickname ( struct Pdu * dgram );
void limpiar_entrada ( );
void pedir_nickname ( struct Pdu * dgram );
void pedir_username ( struct Pdu * dgram );
void pedir_passwdx2 ( struct Pdu * dgram );
void pedir_password ( struct Pdu * dgram );
int registrar ( struct Pdu * dgram );
int esta_registrado ( struct Pdu * dgram );
int existe_usuario ( struct Pdu * dgram );
void to_md5_hex ( char * str, int str_size, char * md5hex );
