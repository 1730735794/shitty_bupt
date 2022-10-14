#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "log.h"

#define MAX_DB_RETURN 16
#define MAX_NAME_LEN 128
#define MAX_PKT_LEN 1024
#define IN_CLASS 0x0001
#define A_RECORD 0x0001
#define NS_RECORD 0x0002
#define PTR_RECORD 0x000c
#define CNAME_RECORD 0x0005
#define AAAA_RECORD 0x001c
#define NO_SUCH_NAME 0b0011

struct data_info //保存报文以及发送方地址
{
    char data[MAX_PKT_LEN];
    struct sockaddr_in addr;
};

int socket_init();
void Recv_msg(int sock, struct data_info *info_ptr);              //在sock上接收报文并把报文存入info_ptr->data中，把发送方的地址存入info_ptr->addr中
void Send_msg(int sock, struct data_info *info_ptr, int pkt_len); //用sock向info_ptr->addr中的地址发送info_ptr->data中的报文
#endif
