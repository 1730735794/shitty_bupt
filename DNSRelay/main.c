#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocol.h"
#include "trietree.h"
#include "log.h"
#include "resolver.h"
#include "id_map.h"

FILE *config(int argc, char *argv[]);
void message_info_print(char *string, struct sockaddr_in *addr_ptr, struct message *message_ptr);

int debug_level;                    //debug信息
struct sockaddr_in dns_server_addr; //外部dns服务器地址

int main(int argc, char *argv[])
{
    FILE *cache_file = config(argc, argv); //参数处理
    int sock = socket_init();              //初始化socket
    struct data_info socket_buffer;        //储存解析前报文和发送方地址
    struct message rs_message;             //储存解析后的报文

    struct node *internal_root = new_tree(); //建立内部cache并添加记录
    add_server_info(internal_root);
    add_record_from_local_file(internal_root, cache_file);

    struct node *external_root = new_tree(); //建立外部cache

    struct id_map *map = new_list();
    u_short server_id_to_send = 10;

    while (1) //消息循环
    {
        Recv_msg(sock, &socket_buffer);                //接收报文
        recv_process(socket_buffer.data, &rs_message); //将字节串转换为结构化形式的报文
        int need_to_free_answer = rs_message.header.answer_num;
        int need_to_free_authority = rs_message.header.authority_num;
        int need_to_free_addition = rs_message.header.addition_num;
        message_info_print("Recv from", &socket_buffer.addr, &rs_message);

        if (resolve(&rs_message, internal_root, external_root) >= 0) //找到记录
        {
            if (socket_buffer.addr.sin_addr.s_addr == dns_server_addr.sin_addr.s_addr)  //服务器回复，查找对应客户端信息并删除映射
            {
                u_short server_id = rs_message.header.ID;
                struct id_map *mapping = search_by_server_id(map, rs_message.header.ID);    //找到对应项
                if (mapping == NULL)
                    continue;
                socket_buffer.addr = mapping->client_addr;
                rs_message.header.ID = mapping->client_id;
                delete_by_server_id(map, server_id);
                clear_outdated_info(map);
            }
        }
        else    //未找到记录，向服务器发送请求
        {
            new_info(map, socket_buffer.addr, rs_message.header.ID, server_id_to_send);    //添加映射
            socket_buffer.addr = dns_server_addr;
            rs_message.header.ID = server_id_to_send;
            server_id_to_send++;
            if (server_id_to_send == 0) //ID为0不合法
                server_id_to_send++;
        }

        int pkt_len = send_process(socket_buffer.data, &rs_message); //将结构化形式的报文转换为字节串
        Send_msg(sock, &socket_buffer, pkt_len);                     //发送报文
        message_info_print("Send to", &socket_buffer.addr, &rs_message);

        free(rs_message.query); //回收解析时申请的内存
        if (need_to_free_answer)
            free(rs_message.answer);
        if (need_to_free_authority)
            free(rs_message.authority);
        if (need_to_free_addition)
            free(rs_message.addition);
    }
    return 0;
}

FILE *config(int argc, char *argv[])
{
    debug_level = 0;
    FILE *cache_file = NULL;
    dns_server_addr.sin_family = AF_INET;
    dns_server_addr.sin_port = htons(53);
    inet_pton(AF_INET, "114.114.114.114", &dns_server_addr.sin_addr.s_addr);
    char ch;
    char filename[64] = "dnsrelay.txt";
    while ((ch = getopt(argc, argv, "d::i:f:h?")) != -1)
    {
        switch (ch)
        {
        case 'd':
            if (optarg == NULL)
                debug_level = 1;
            else if (strcmp(optarg, "d") == 0)
                debug_level = 2;
            else
                fatal_exit_print("Wrong debug level");
            break;
        case 'i':
            if (inet_pton(AF_INET, optarg, &dns_server_addr.sin_addr.s_addr) != 1)
                fatal_exit_print("Wrong ip address");
            break;
        case 'f':
            strcpy(filename, optarg);
            break;
        case 'h':
        case '?':
            printf("Help:\n");
            printf("\tdnsrelay [-d|-dd] [-i dns-server-ipaddr] [-f filename]\n");
            exit(0);
            break;
        }
    }
    if ((cache_file = fopen(filename, "r")) == NULL)
        fatal_exit_print("Wrong filename");
    char ip_addr[64];
    inet_ntop(AF_INET, &dns_server_addr.sin_addr.s_addr, ip_addr, 64);
    printf("\nThis program is on https://github.com/Direktor799/21_ComputerNetworkCourseProject_DNSRelay\n\n");
    printf("debug level = %d\n", debug_level);
    printf("external DNS address = %s\n", ip_addr);
    printf("local file = %s\n\n", filename);
    return cache_file;
}

void message_info_print(char *string, struct sockaddr_in *addr_ptr, struct message *message_ptr)
{
    char result[200] = "";
    strcat(result, string);
    strcat(result, " ");
    char addr_string[64];
    inet_ntop(AF_INET, &addr_ptr->sin_addr.s_addr, addr_string, 64);
    strcat(result, addr_string);
    if (strlen(result) < 19)
        strcat(result, "\t");
    strcat(result, "\t[");
    if (message_ptr->header.QR == 0)
        strcat(result, "Query ");
    else
        strcat(result, "Response ");
    char id[8];
    sprintf(id, "0x%04x ", message_ptr->header.ID);
    strcat(result, id);
    if (message_ptr->query->type == A_RECORD)
        strcat(result, "A ");
    else if (message_ptr->query->type == AAAA_RECORD)
        strcat(result, "AAAA ");
    else if (message_ptr->query->type == PTR_RECORD)
        strcat(result, "PTR ");
    else if (message_ptr->query->type == CNAME_RECORD)
        strcat(result, "CNAME ");
    else
        strcat(result, "Unknown ");
    char name[MAX_NAME_LEN];
    to_with_dot(name, message_ptr->query->name);
    strcat(result, name);
    strcat(result, "]");
    info_print(result);
}