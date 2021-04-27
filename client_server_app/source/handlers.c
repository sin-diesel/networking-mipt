#include "my_server.h"


pthread_mutex_t mutexes[MAXCLIENTS] = {0};
pthread_mutex_t guard_mutexes[MAXCLIENTS] = {0};
int client_sockets[MAXCLIENTS];
int packets_counter[MAXCLIENTS];

void* tcp_handle_connection(void* memory) {

    LOG("Entered new thread (TCP)%s\n", "");

    struct message msg;
    int ret = 0;
    char dir[MAXPATH];
    char buf[BUFSIZ];
    char* dirp;
    int client_sk = *((int*) memory);
    
    LOG("Client sk in handler: %d\n", client_sk);

    /* Init client directory*/
    dirp = getcwd(dir, MAXPATH);
    if (dirp == NULL) {
        LOG("Error changing directory: %s\n", strerror(errno));
        ERROR(errno);
        exit(EXIT_FAILURE);
    }
    LOG("Current thread directory: %s\n", dir);

    while (1) {

        //pthread_mutex_lock(&mutexes[0]);

        LOG("Reading from client_sk %d\n", client_sk);
        ret = read(client_sk, &msg, sizeof(struct message));
        LOG("Message read.%s\n", "");
        if (ret < 0) {
            LOG("Error reading from client socket %d\n", client_sk);
            ERROR(errno);
            exit(EXIT_FAILURE);
        }
        /* Process message */
        print_info(&msg);

        char* addr = inet_ntoa(msg.client_data.sin_addr);
        if (addr == NULL) {
            LOG("Client address invalid %s\n", "");
        }
        LOG("Client address: %s\n", addr);
        LOG("Client port: %d\n", msg.client_data.sin_port);

        /* Handle client's command */   
        ret = handle_message(&msg, dir, buf);
        if (ret < 0) {
            exit(EXIT_FAILURE);
        }
        ret = tcp_reply_to_client(client_sk, &msg);
        if (ret < 0) {
            exit(EXIT_FAILURE);
        }
    }

}

//---------------------------------------------------
void* udp_handle_connection(void* memory) {

    struct message msg;
    int current_packet = 0;
    int ret = 0;

    /* Buffer for maintaining data */
    char buf[BUFSIZ];
    // char ack[] = "Message received";
    // char none[] = "None";
    char dir[MAXPATH];
    memset(&msg, 0, sizeof(struct message));
    memset(buf, 0, BUFSIZ);

    /* Construct default ack message to client */
    // memcpy(msg.cmd, none, sizeof(none));
    // memcpy(msg.data, ack, sizeof(ack));

    LOG("Entered new thread %s\n", "");

    /* Init client directory*/
    char* dirp = getcwd(dir, MAXPATH);
    if (dirp == NULL) {
        LOG("Error changing directory: %s\n", strerror(errno));
        ERROR(errno);
        exit(EXIT_FAILURE);
    }
    LOG("Current thread directory: %s\n", dir);
    /* Copy data from memory */
    memcpy(&msg, memory, sizeof(struct message));

    while (1) {
        /* Copy data from memory */
        /* Lock mutex */
        LOG("Waiting for mutex to be unlocked%s\n", "");
        pthread_mutex_lock(&mutexes[msg.id]);

        pthread_mutex_lock(&guard_mutexes[msg.id]);
        memcpy(&msg, memory, sizeof(struct message));
        LOG("Guard mutex unlocked%s\n", "");
        pthread_mutex_unlock(&guard_mutexes[msg.id]);

        print_info(&msg);
        ret = print_client_addr(&msg);
        if (ret < 0) {
            LOG("Client address invalid %s\n", "");
        }

        /* Handle client's command */
        ret = handle_message(&msg, dir, buf);
        if (ret < 0) {
            exit(EXIT_FAILURE);
        }

        current_packet = msg.packet_number;
        LOG("Packet number received: %d\n", current_packet);
        LOG("Expected packet: %d\n", packets_counter[msg.id]);
        if (current_packet != packets_counter[msg.id]) {
            LOG("Error in packet transmission. Expected packet: %d, received: %d\n", packets_counter[msg.id], current_packet)
        }

        //packets_counter[msg.id]++;
        msg.packet_number = packets_counter[msg.id];
        packets_counter[msg.id]++;
        ret = reply_to_client(&msg);

        if (ret < 0) {
            exit(EXIT_FAILURE);
        }
    }


    return NULL;
}