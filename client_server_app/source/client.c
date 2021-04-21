
#include "my_server.h"


int main(int argc, char** argv) {

    /* UDP connection by default */
    int connection_type = UDP_CON;
    int sk = 0;
    struct sockaddr_in sk_addr;
    struct sockaddr_in sk_broad;
    struct sockaddr_in sk_bind;
    char* ip_addr = NULL;
    struct sockaddr_in server_data;
    struct client_info info;

    int ret = 0;

    client_check_input(argc, argv, &connection_type, ip_addr);

    ret = client_init(connection_type, &sk, &sk_addr, &sk_bind, &sk_broad, ip_addr, &info);
    if (ret < 0) {
        exit(EXIT_FAILURE);
    }
    ret = client_routine(&info, &server_data);
    if (ret < 0) {
        exit(EXIT_FAILURE);
    }
      
    return 0;
}