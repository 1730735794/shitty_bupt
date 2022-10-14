#ifndef ID_MAP_H
#define ID_MAP_H
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocol.h"
#include "log.h"
#include "resolver.h"

struct id_map
{
    struct sockaddr_in client_addr;
    u_short client_id;
    u_short server_id;
    int set_up_time;
    struct id_map *next;
};

struct id_map *new_list();  //新建id对照表
void new_info(struct id_map *head, struct sockaddr_in cli_addr, u_short cli_id, u_short svr_id);  //新增记录
struct id_map *search_by_server_id(struct id_map *head, u_short server_id); //搜索对应的客户端信息
void delete_by_server_id(struct id_map *head, u_short svr_id);
void clear_outdated_info(struct id_map *head);
#endif