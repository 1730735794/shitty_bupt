#ifndef RESOLVER_H
#define RESOLVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "protocol.h"
#include "trietree.h"

struct header
{
    u_short ID;       //会话标识
    u_int QR : 1;     //查询/响应标志
    u_int opcode : 4; //查询响应类型
    u_int AA : 1;     //授权回答 authoritative answer
    u_int TC : 1;     //截断标志 truncated
    u_int RD : 1;     //期望递归标志 recursion desired
    u_int RA : 1;     //可用递归标志 recursion available
    u_int nil : 3;    //必须为0
    u_int rcode : 4;  //返回码
    u_short query_num;
    u_short answer_num;
    u_short authority_num;
    u_short addition_num;
};

struct message
{
    struct header header;
    struct query *query;
    struct RR *answer;
    struct RR *authority;
    struct RR *addition;
};
int get_name_flag(u_char * name);//获取名字中标识符个数,不包含末尾0
void get_reverse_nameflag( char * reverse_name_flag, u_char * name, int flag_num);
//获取name中的计数标识符(逆转后，不包含末尾0，第一字节为0在name中的位置,第二字节为倒数第一个计数符)
//3www5testt4qnmb0运行后,reverse_name_flag[0] = 15,xxxx[1] = 4, xxxx[2] = 5,xxxx[3] = 3

int send_name(u_char *buffer, int bias, u_char * name);
int get_name(u_char *des, u_char *buffer, int bias); //处理buf中bias位置的（压缩格式）字符串并存入des中，返回字符串后的位置
void header_recv_process(u_char *buffer, struct header *h);
int query_recv_process(u_char *buffer, int bias, struct query *q);
int RR_recv_process(u_char *buffer, int bias, struct RR *rr);
void recv_process(u_char *buffer, struct message *m);
void header_send_process(u_char *buffer, struct header *h);
int query_send_process(u_char *buffer, int bias, struct query *q);
int RR_send_process(u_char *buffer, int bias, struct RR *rr);
int send_process(u_char *buffer, struct message *m); //返回包长度
int resolve(struct message *rs_message, struct node *internal_cache, struct node *external_cache);
#endif