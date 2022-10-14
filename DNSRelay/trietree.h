#ifndef TRIETREE_H
#define TRIETREE_H
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "protocol.h"

struct query //查询
{
    u_char name[MAX_NAME_LEN];
    u_short type;
    u_short class;
};

struct RR //资源记录
{
    struct query RR_query;
    u_int TTL;
    u_short RD_len;
    u_char RD_data[MAX_NAME_LEN];
};

struct node_data
{
    u_short type;  //记录类型
    u_char *data;  //数据
    int TTL;       //生存周期
    int recv_time; //到达时间
    struct node_data *next;
};

struct node //trie树结点
{
    char *key;                //结点键值
    struct node_data *record; //结点记录
    struct node *parent;      //上层指针
    struct node *brother;     //同层指针
    struct node *child;       //下层指针
};
//结构相关
struct node *_new_node(char *key_string, int string_len);                      //建立结点，设置结点键值（在key_string基础上加上‘\0’结束符）并清空结点记录（length为键值长度）
void _set_as_child(struct node *parent_ptr, struct node *child_ptr);       //将一个结点设置为另一个结点的子结点
struct node *_add_child(struct node *node_ptr, char *key_string, int string_len);  //建立key为key_string（长度为string_len)的结点，并设为node_ptr的子结点
struct node *_find_child(struct node *node_ptr, char *key_string, int string_len); //找到node_ptr的key为key_string的子结点
struct node *add_node_in_tree(struct node *root, char *key_string, int string_len);  //向树中添加结点
struct node *find_node_in_tree(struct node *root, char *key_string, int string_len); //在树中找到结点
void clear_outdated_data(struct node *node_ptr);                             //删除过期数据
struct node *new_tree();
//工具函数
int to_with_dot(char *des, char *src); //将数字分隔的域名字符串转化为点分隔字符串，返回长度
int to_with_num(char *des, char *src); //将点分隔的域名字符串转化为数字分隔字符串，返回长度
char num_to_char(uint8_t num);         //将num的值转化为对应的十六进制数

//外部接口
void add_node_data(struct node *node_ptr, u_short type_of_data, char *data_string, int time_to_live); //增加结点数据
void add_server_info(struct node *root);                                                              //添加本机的记录到数据库中
int add_record_from_local_file(struct node *root, FILE *fp);                                          //从本地文件中读取信息并建树
int query_in_database(struct node *root, struct query *question, struct RR *answer, int now_answer);
#endif