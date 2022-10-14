#include "trietree.h"

struct node *_new_node(char *key_string, int length)
{
    struct node *node_ptr = malloc(sizeof(struct node));
    node_ptr->parent = NULL;
    node_ptr->brother = NULL;
    node_ptr->child = NULL;
    node_ptr->key = malloc(length + 1);
    memcpy(node_ptr->key, key_string, length + 1);
    node_ptr->key[length] = '\0';
    node_ptr->record = NULL;
    return node_ptr;
}

void _set_as_child(struct node *parent_ptr, struct node *child_ptr)
{
    child_ptr->parent = parent_ptr;
    if (parent_ptr->child == NULL)
        parent_ptr->child = child_ptr;
    else
    {
        struct node *p = parent_ptr->child;
        while (p->brother != NULL)
            p = p->brother;
        p->brother = child_ptr;
    }
}

struct node *_add_child(struct node *node_ptr, char *string, int length)
{
    struct node *child_ptr = _new_node(string, length);
    _set_as_child(node_ptr, child_ptr);
    return child_ptr;
}

struct node *_find_child(struct node *node_ptr, char *string, int length)
{
    struct node *p = node_ptr->child;
    while (p != NULL)
    {
        if (strlen(p->key) == length && strncmp(p->key, string, length) == 0)
            break;
        p = p->brother;
    }
    return p;
}

struct node *new_tree()
{
    return _new_node("", 0);
}

struct node *add_node_in_tree(struct node *root, char *string, int length)
{
    int end = length - 1;
    while (string[end] != '.' && end >= 0)
        end--;
    struct node *child = _find_child(root, string + end + 1, length - 1 - end);
    if (child == NULL)
        child = _add_child(root, string + end + 1, length - 1 - end);
    if (end >= 0)
        return add_node_in_tree(child, string, end);
    else
        return child;
}

struct node *find_node_in_tree(struct node *root, char *string, int length)
{
    int end = length - 1;
    while (string[end] != '.' && end >= 0)
        end--;
    struct node *child = _find_child(root, string + end + 1, length - 1 - end);
    if (child == NULL)
        return NULL;
    if (end >= 0)
        return find_node_in_tree(child, string, end);
    else
        return child;
}

void add_node_data(struct node *node_ptr, u_short type_of_data, char *data_string, int time_to_live)
{
    struct node_data *new_record = malloc(sizeof(struct node_data));
    new_record->type = type_of_data;
    if (type_of_data == A_RECORD)
    {
        new_record->data = malloc(4);
        memcpy(new_record->data, data_string, 4);
    }
    else if (type_of_data == AAAA_RECORD)
    {
        new_record->data = malloc(16);
        memcpy(new_record->data, data_string, 16);
    }
    else if (type_of_data == PTR_RECORD || type_of_data == CNAME_RECORD)
    {
        new_record->data = malloc(strlen(data_string) + 1);
        memcpy(new_record->data, data_string, strlen(data_string) + 1);
    }

    new_record->TTL = time_to_live;
    new_record->recv_time = time(NULL);
    new_record->next = NULL;

    if (node_ptr->record == NULL)
        node_ptr->record = new_record;
    else
    {
        struct node_data *p = node_ptr->record;
        while (p->next != NULL)
            p = p->next;
        p->next = new_record;
    }
}

void clear_outdated_data(struct node *node_ptr)
{
    if (node_ptr->record == NULL)
        return;
    struct node_data *p = node_ptr->record;
    while (p != NULL && p->TTL != -1 && p->recv_time + p->TTL < time(NULL)) //第一个数据过期
    {
        struct node_data *to_be_deleted = p;
        p = p->next;
        free(to_be_deleted);
    }
    node_ptr->record = p;                                         //确保第一个数据未过期或为空
    if (p == NULL)
        return;
    while (p->next != NULL && p->next->TTL != -1 && p->next->recv_time + p->next->TTL < time(NULL)) //处理后续数据
    {
        struct node_data *to_be_deleted = p->next;
        p->next = p->next->next;
        free(to_be_deleted);
    }
}

void add_server_info(struct node *root)
{
    char my_name[MAX_NAME_LEN] = "dns.9senko.org";

    FILE *pFile = popen("ip addr | grep eth0 | grep inet | awk '{print substr($2, 1, length($2) - 3)}'", "r");
    char ipv4_string[MAX_NAME_LEN];
    fgets(ipv4_string, sizeof(ipv4_string), pFile);
    ipv4_string[strlen(ipv4_string) - 1] = '\0';
    pclose(pFile);

    //构造自己的A记录
    struct in_addr ipv4;
    inet_pton(AF_INET, ipv4_string, &ipv4.s_addr);
    struct node *new_node = add_node_in_tree(root, my_name, strlen(my_name));
    add_node_data(new_node, A_RECORD, (char *)&ipv4.s_addr, -1);

    //构造自己的ipv4PTR记录
    struct in_addr reverse_addr;
    reverse_addr.s_addr = ntohl(ipv4.s_addr);
    char tmp[MAX_NAME_LEN];
    inet_ntop(AF_INET, &reverse_addr.s_addr, tmp, MAX_NAME_LEN);
    strcat(tmp, ".in-addr.arpa");
    new_node = add_node_in_tree(root, tmp, strlen(tmp));
    to_with_num(tmp, my_name);
    add_node_data(new_node, PTR_RECORD, tmp, -1);

    pFile = popen("ip addr | grep inet6 | awk 'NR==2 {print substr($2, 1, length($2) - 3)}'", "r");
    char ipv6_string[MAX_NAME_LEN];
    fgets(ipv6_string, sizeof(ipv6_string), pFile);
    ipv6_string[strlen(ipv6_string) - 1] = '\0';
    pclose(pFile);

    //构造自己的AAAA记录
    struct in6_addr ipv6;
    inet_pton(AF_INET6, ipv6_string, &ipv6);
    new_node = add_node_in_tree(root, my_name, strlen(my_name));
    add_node_data(new_node, AAAA_RECORD, (char *)&ipv6, -1);

    //构造自己的ipv6PTR记录
    char ipv6_ptr_name[MAX_NAME_LEN];
    for (int i = 0; i < 16; i++)
    {
        ipv6_ptr_name[i * 2 * 2] = num_to_char(ipv6.__in6_u.__u6_addr8[16 - i - 1] & 0b1111);
        ipv6_ptr_name[i * 2 * 2 + 1] = '.';
        ipv6_ptr_name[i * 2 * 2 + 2] = num_to_char(ipv6.__in6_u.__u6_addr8[16 - i - 1] >> 4);
        ipv6_ptr_name[i * 2 * 2 + 3] = '.';
    }
    ipv6_ptr_name[64] = '\0';
    strcat(ipv6_ptr_name, "ip6.arpa");
    new_node = add_node_in_tree(root, ipv6_ptr_name, strlen(ipv6_ptr_name));
    to_with_num(ipv6_ptr_name, my_name);
    add_node_data(new_node, PTR_RECORD, ipv6_ptr_name, -1);
}

int add_record_from_local_file(struct node *root, FILE *fp)
{
    char *line_buffer = NULL;
    size_t length = 0;
    struct in6_addr ipv6;
    struct in_addr ipv4;
    while (getline(&line_buffer, &length, fp) > 0)
    {
        if (line_buffer[0] == '#' || line_buffer[0] == '\r' || line_buffer[0] == '\n') //'#'开头为注释
            continue;
        int div = 0;
        while (line_buffer[div] != ' ')
            div++;
        line_buffer[div] = '\0';
        div++;
        while (line_buffer[div] == ' ')
            div++;
        int str_length = strlen(line_buffer + div);
        while (line_buffer[div + str_length - 1] == '\r' || line_buffer[div + str_length - 1] == '\n' || line_buffer[div + str_length - 1] == ' ')
        {
            line_buffer[div + str_length - 1] = '\0';
            str_length--;
        }

        if (inet_pton(AF_INET, line_buffer + div, &ipv4.s_addr) == 1) //A记录
        {
            struct node *new_node = add_node_in_tree(root, line_buffer, strlen(line_buffer));
            add_node_data(new_node, A_RECORD, (char *)&ipv4.s_addr, -1);
        }
        else if (inet_pton(AF_INET6, line_buffer + div, &ipv6) == 1) //AAAA记录
        {
            struct node *new_node = add_node_in_tree(root, line_buffer, strlen(line_buffer));
            add_node_data(new_node, AAAA_RECORD, (char *)&ipv6, -1);
        }
        else //PTR记录
        {
            if (inet_pton(AF_INET, line_buffer, &ipv4.s_addr) == 1) //ipv4
            {
                struct in_addr reverse_ipv4;
                reverse_ipv4.s_addr = ntohl(ipv4.s_addr);
                char ipv4_ptr_name[MAX_NAME_LEN];
                inet_ntop(AF_INET, &reverse_ipv4.s_addr, ipv4_ptr_name, MAX_NAME_LEN);
                strcat(ipv4_ptr_name, ".in-addr.arpa");
                struct node *new_node = add_node_in_tree(root, ipv4_ptr_name, strlen(ipv4_ptr_name));
                to_with_num(ipv4_ptr_name, line_buffer + div);
                add_node_data(new_node, PTR_RECORD, ipv4_ptr_name, -1);
            }
            else if (inet_pton(AF_INET6, line_buffer, &ipv6) == 1) //ipv6
            {
                char ipv6_ptr_name[MAX_NAME_LEN];
                for (int i = 0; i < 16; i++)
                {
                    ipv6_ptr_name[i * 2 * 2] = num_to_char(ipv6.__in6_u.__u6_addr8[16 - i - 1] & 0b1111);
                    ipv6_ptr_name[i * 2 * 2 + 1] = '.';
                    ipv6_ptr_name[i * 2 * 2 + 2] = num_to_char(ipv6.__in6_u.__u6_addr8[16 - i - 1] >> 4);
                    ipv6_ptr_name[i * 2 * 2 + 3] = '.';
                }
                ipv6_ptr_name[64] = '\0';
                strcat(ipv6_ptr_name, "ip6.arpa");
                struct node *new_node = add_node_in_tree(root, ipv6_ptr_name, strlen(ipv6_ptr_name));
                to_with_num(ipv6_ptr_name, line_buffer + div);
                add_node_data(new_node, PTR_RECORD, ipv6_ptr_name, -1);
            }
            else //CNAME记录
            {
                char name[MAX_NAME_LEN];
                to_with_num(name, line_buffer + div);
                struct node *new_node = add_node_in_tree(root, line_buffer, strlen(line_buffer));
                add_node_data(new_node, CNAME_RECORD, name, -1);
            }
        }
        free(line_buffer);
        line_buffer = NULL;
    }
    fclose(fp);
}

int query_in_database(struct node *root, struct query *question, struct RR answer[], int now_answer)
{
    int answer_num = now_answer;

    //将请求域名转化为带.的格式存入buffer
    u_char buffer[MAX_NAME_LEN];
    to_with_dot(buffer, question->name);

    //找到该结点
    struct node *answer_node = find_node_in_tree(root, buffer, strlen(buffer));
    struct node_data *answer_data;
    if (answer_node == NULL)
        return answer_num;
    else
    {
        clear_outdated_data(answer_node); //遍历该节点所有数据，删除过期项
        answer_data = answer_node->record;
    }
    //查询该节点中所有数据
    while (answer_data != NULL && answer_num < MAX_DB_RETURN)
    {
        if (answer_data->type == A_RECORD && *(int *)answer_data->data == 0)
            return -1;
        if (answer_data->type == CNAME_RECORD || answer_data->type == question->type)
        {
            if (answer_data->type == CNAME_RECORD)
                answer_num = now_answer;
            //对所有类型的公共处理
            answer[answer_num].RR_query.class = IN_CLASS;
            if (answer_data->TTL != -1)
                answer[answer_num].TTL = answer_data->TTL;
            else
                answer[answer_num].TTL = 86400;
            strcpy(answer[answer_num].RR_query.name, question->name);
            answer[answer_num].RR_query.type = answer_data->type;

            if (answer_data->type == CNAME_RECORD)
            {
                answer[answer_num].RD_len = strlen(answer_data->data) + 1;
                memcpy(answer[answer_num].RD_data, answer_data->data, answer[answer_num].RD_len);
                struct query next_query;
                next_query.class = IN_CLASS;
                next_query.type = question->type;
                strcpy(next_query.name, answer[answer_num].RD_data);
                return query_in_database(root, &next_query, answer, answer_num + 1);
            }
            else if (answer_data->type == question->type)
            {
                //对各类型分开处理
                if (answer_data->type == A_RECORD)
                {
                    answer[answer_num].RD_len = 4;
                    memcpy(answer[answer_num].RD_data, answer_data->data, answer[answer_num].RD_len);
                }
                else if (answer_data->type == AAAA_RECORD)
                {
                    answer[answer_num].RD_len = 16;
                    memcpy(answer[answer_num].RD_data, answer_data->data, answer[answer_num].RD_len);
                }
                else if (answer_data->type == PTR_RECORD)
                {
                    answer[answer_num].RD_len = strlen(answer_data->data) + 1;
                    memcpy(answer[answer_num].RD_data, answer_data->data, answer[answer_num].RD_len);
                }
            }
            answer_num++;
        }
        answer_data = answer_data->next;
    }

    return answer_num;
}

int to_with_dot(char *des, char *src)
{
    int char_cnt = src[0];
    int pos = 0;
    while (char_cnt > 0)
    {
        for (int i = 0; i < char_cnt; i++)
        {
            des[pos] = src[pos + 1];
            pos++;
        }
        char_cnt = src[pos + 1];
        des[pos] = '.';
        pos++;
    }
    des[pos - 1] = '\0';
    return pos - 1;
}

int to_with_num(char *des, char *src)
{
    int src_len = strlen(src);
    int pos = 0;
    int part_len = 0;
    while (pos < src_len)
    {
        while (src[pos] != '.' && pos < src_len)
        {
            des[pos + 1] = src[pos];
            part_len++;
            pos++;
        }
        des[pos - part_len] = part_len;
        part_len = 0;
        pos++;
    }
    des[pos] = 0x0;
    return pos + 1;
}

char num_to_char(uint8_t num)
{
    if (num < 0xa)
        num += '0';
    else
        num += 'a' - 0xa;
    return num;
}