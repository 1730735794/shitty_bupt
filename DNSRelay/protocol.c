#include "protocol.h"
int socket_init()
{
    //创建套接字
    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        fatal_exit_print("Socket init error");

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(53);
    if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr)) < 0)
        fatal_exit_print("Socket bind error");
    return sock;
}

void Recv_msg(int sock, struct data_info *info_ptr)
{
    socklen_t addr_len = sizeof(struct sockaddr_in);
    memset(info_ptr->data, 0, MAX_PKT_LEN);
    if (recvfrom(sock, &info_ptr->data, MAX_PKT_LEN, 0, (struct sockaddr *)&info_ptr->addr, &addr_len) < 0)
        fatal_exit_print("Message recv error");
    return;
}

void Send_msg(int sock, struct data_info *info_ptr, int pkt_len)
{
    if (sendto(sock, &info_ptr->data, pkt_len, 0, (struct sockaddr *)&info_ptr->addr, sizeof(struct sockaddr_in)) < 0)
        fatal_exit_print("Message send error");
    return;
}
