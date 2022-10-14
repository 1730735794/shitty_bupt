#include "resolver.h"
int get_name_flag(u_char *name) //获取名字中标识符个数,不包含末尾0
{
    int name_len = 0; //名字长度
    int flag_num = 0; //标志名字片段长度
    while (name[name_len] && name_len < MAX_NAME_LEN)
    {
        name_len += name[name_len] + 1;
        flag_num++;
    }
    flag_num++;
    return flag_num;
}

void get_reverse_nameflag(char *reverse_name_flag, u_char *name, int flag_num)
{
    int name_len = 0;
    for (int i = flag_num - 1; i > 0; i--)
    {
        reverse_name_flag[i] = name[name_len];
        name_len += name[name_len] + 1;
    }
    name_len++;
    reverse_name_flag[0] = name_len;

    // for(int i = 0; i < flag_num; i++)
    //     printf("%d ",reverse_name_flag[i]);
}

int send_name(u_char *buffer, int bias, u_char *name)
{
    int flag_num = get_name_flag(name);
    char *reverse_name_flag = malloc(flag_num);
    memset(reverse_name_flag, 0, flag_num);
    get_reverse_nameflag(reverse_name_flag, name, flag_num);

    int name_length = reverse_name_flag[0];
    int buf_cmp = 12;         //标志下一个需要匹配的字节在buf中的位置
    u_short name_ptr;         //标志当前已查找到的最优name的后缀指针
    int ptr_flag = 0;         //标志是否有name指针
    int back_name_length = 0; //标志当前匹配成功的后缀字节数
    int cur_name_length = 0;
    int cur_seg_length = 0;
    u_char *final_name;
    int final_name_len = 0;
    for (int i = 1; i < flag_num; i++)
    {

        char cmp_name[MAX_NAME_LEN];
        cur_seg_length = reverse_name_flag[i] + 1;
        if (i == 1)
            cur_seg_length += 1; //如果是倒数第一个字节，要加上末尾的0这一字节
        //初始化当前要匹配的字段
        if (ptr_flag)
        {
            cur_name_length = cur_seg_length + 2;
            memcpy(cmp_name, name + name_length - back_name_length - cur_seg_length, cur_seg_length);
            u_short tmp_ptr = htons(name_ptr);
            memcpy(cmp_name + cur_seg_length, (u_char *)&tmp_ptr, 2);
        }
        else
        {
            cur_name_length = cur_seg_length;
            memcpy(cmp_name, name + name_length - back_name_length - cur_seg_length, cur_seg_length);
        }

        //匹配当前字段
        while (strncmp(buffer + buf_cmp, cmp_name, cur_name_length) != 0 && (buf_cmp + cur_name_length) < bias)
            buf_cmp++;


        //如果匹配成功,匹配下一个名字片段。如3www5baidu3com0,3com0匹配成功,则在buf当前位置往前6位匹配5baidu
        if (strncmp(buffer + buf_cmp, cmp_name, cur_name_length) == 0)
        {
            back_name_length += cur_seg_length;
            ptr_flag = 1;
            while (i < flag_num - 1)
            {
                cur_seg_length = reverse_name_flag[i + 1] + 1; //当前匹配的名字片段长度
                if (strncmp(buffer + buf_cmp - cur_seg_length, name + name_length - back_name_length - cur_seg_length, cur_seg_length) == 0)
                {
                    buf_cmp -= cur_seg_length;          //buf前移
                    back_name_length += cur_seg_length; //已匹配成功的名字长度增加
                    i++;
                }
                else
                    break;
            }
            name_ptr = 0b11 << 14 | buf_cmp; //计算最终name_ptr
        }
        //如果匹配失败,buf已匹配至尽头,退出循环
        else
            break;
    }

    //最终得到的名字的赋值
    if (ptr_flag)
    {
        final_name_len = name_length - back_name_length + 2;
        final_name = malloc(final_name_len);
        memcpy(final_name, name, name_length - back_name_length);
        u_short tmp = htons(name_ptr);
        memcpy(final_name + name_length - back_name_length, (u_char *)&tmp, 2);
    }
    else
    {
        final_name_len = name_length - back_name_length;
        final_name = malloc(final_name_len);
        memcpy(final_name, name, name_length - back_name_length);
    }

    memcpy(buffer + bias, final_name, final_name_len);
    free(reverse_name_flag);
    free(final_name);
    return bias + final_name_len;
    // for (int j = 0; j < name_length - back_name_length + 2; j++)
    //     printf("%x ", final_name[j]);
}

int get_name(u_char *des, u_char *buffer, int bias)
{
    memset(des, 0, MAX_NAME_LEN);
    int len = 0;
    int flag = 0; //标志是否出现过指针，0为未出现过，1为出现过
    int have_ptr_bias = 0;
    while (buffer[bias] != 0)
    {
        if (buffer[bias] >= 0xc0) //最高两位为11
        {
            if (!flag)
                have_ptr_bias = bias + 2; //计算与初始偏移量相比的偏移量
            flag = 1;
            bias = ((buffer[bias] & 0b111111) << 8) + buffer[bias + 1];
        }
        else
        {
            memcpy(des + len, buffer + bias, buffer[bias] + 1);
            len += buffer[bias] + 1;
            bias += buffer[bias] + 1;
        }
    }
    des[len] = 0x0;
    if (flag)
        return have_ptr_bias;
    else
        return bias + 1;
}

void header_recv_process(u_char *buffer, struct header *h)
{
    h->ID = ntohs(*(u_short *)buffer);
    h->QR = buffer[2] >> 7;
    h->opcode = (buffer[2] >> 3) & 0b01111;
    h->AA = (buffer[2] >> 2) & 0b000001;
    h->TC = (buffer[2] & 0b10) >> 1;
    h->RD = buffer[2] & 0b1;
    h->RA = buffer[3] >> 7;
    h->nil = (buffer[3] >> 4) & 0b111;
    h->rcode = buffer[3] & 0b1111;
    h->query_num = ntohs(*(u_short *)(buffer + 4));
    h->answer_num = ntohs(*(u_short *)(buffer + 6));
    h->authority_num = ntohs(*(u_short *)(buffer + 8));
    h->addition_num = ntohs(*(u_short *)(buffer + 10));
}

int query_recv_process(u_char *buffer, int bias, struct query *q)
{
    bias = get_name((u_char *)&q->name, buffer, bias);
    q->type = ntohs(*(u_short *)(buffer + bias));
    bias += 2;
    q->class = ntohs(*(u_short *)(buffer + bias));
    bias += 2;
    return bias;
}

int RR_recv_process(u_char *buffer, int bias, struct RR *rr)
{
    int len = 0; //tmp for RD_len
    bias = query_recv_process(buffer, bias, &rr->RR_query);
    rr->TTL = ntohl(*(u_long *)(buffer + bias));
    bias += 4;
    len = rr->RD_len = ntohs(*(u_short *)(buffer + bias));
    bias += 2;
    if (rr->RR_query.type == CNAME_RECORD || rr->RR_query.type == PTR_RECORD || rr->RR_query.type == NS_RECORD)
        len = get_name((u_char *)&rr->RD_data, buffer, bias);
    else if (rr->RR_query.type == A_RECORD || rr->RR_query.type == AAAA_RECORD)
        memcpy((u_char *)&rr->RD_data, buffer + bias, rr->RD_len);
    bias += rr->RD_len;
    rr->RD_len = len;
    return bias;
}

void recv_process(u_char *buffer, struct message *m)
{
    u_short bias = 0;
    //header
    header_recv_process(buffer, &m->header);
    bias += 12;
    //query
    m->query = malloc(sizeof(struct query) * m->header.query_num);
    for (int i = 0; i < m->header.query_num; i++)
        bias = query_recv_process(buffer, bias, m->query + i);
    //answer
    if (m->header.answer_num)
    {
        m->answer = malloc(sizeof(struct RR) * m->header.answer_num);
        for (int i = 0; i < m->header.answer_num; i++)
            bias = RR_recv_process(buffer, bias, m->answer + i);
    }
    //authority
    if (m->header.authority_num)
    {
        m->authority = malloc(sizeof(struct RR) * m->header.authority_num);
        for (int i = 0; i < m->header.authority_num; i++)
            bias = RR_recv_process(buffer, bias, m->authority + i);
    }
    //addition
    if (m->header.addition_num)
    {
        m->addition = malloc(sizeof(struct RR) * m->header.addition_num);
        for (int i = 0; i < m->header.addition_num; i++)
            bias = RR_recv_process(buffer, bias, m->addition + i);
    }
}

void header_send_process(u_char *buffer, struct header *h)
{
    u_short tmp;
    tmp = htons(h->ID);
    memcpy(buffer, (u_char *)&tmp, 2);
    buffer[2] |= h->QR << 7;
    buffer[2] |= h->opcode << 3;
    buffer[2] |= h->AA << 2;
    buffer[2] |= h->TC << 1;
    buffer[2] |= h->RD;
    buffer[3] |= h->RA << 7;
    buffer[3] |= h->nil << 4;
    buffer[3] |= h->rcode;
    tmp = htons(h->query_num);
    memcpy(buffer + 4, (u_char *)&tmp, 2);
    tmp = htons(h->answer_num);
    memcpy(buffer + 6, (u_char *)&tmp, 2);
    tmp = htons(h->authority_num);
    memcpy(buffer + 8, (u_char *)&tmp, 2);
    tmp = htons(h->addition_num);
    memcpy(buffer + 10, (u_char *)&tmp, 2);
}

int query_send_process(u_char *buffer, int bias, struct query *q)
{
    u_short tmp;
    bias = send_name(buffer, bias, (u_char *)&q->name);
    tmp = htons(q->type);
    memcpy(buffer + bias, (u_char *)&tmp, 2);
    bias += 2;
    tmp = htons(q->class);
    memcpy(buffer + bias, (u_char *)&tmp, 2);
    bias += 2;
    return bias;
}

int RR_send_process(u_char *buffer, int bias, struct RR *rr)
{
    u_short tmps;
    u_long tmpl;
    bias = query_send_process(buffer, bias, &rr->RR_query);
    tmpl = htonl(rr->TTL);
    memcpy(buffer + bias, (u_char *)&tmpl, 4);
    bias += 4;
    int RD_len_bias = bias;
    bias += 2;
    int bias_tmp = bias; //tmp for bias
    if (rr->RR_query.type == CNAME_RECORD || rr->RR_query.type == PTR_RECORD || rr->RR_query.type == NS_RECORD)
    {
        bias = send_name(buffer, bias, rr->RD_data);
        rr->RD_len = bias - bias_tmp;
    }
    else if (rr->RR_query.type == A_RECORD || rr->RR_query.type == AAAA_RECORD)
    {
        memcpy(buffer + bias, rr->RD_data, rr->RD_len);
        bias += rr->RD_len;
    }
    tmps = htons(rr->RD_len); //重写长度
    memcpy(buffer + RD_len_bias, (u_char *)&tmps, 2);
    return bias;
}

int send_process(u_char *buf, struct message *m)
{
    u_int bias = 0;
    memset(buf, 0, MAX_PKT_LEN);
    //header
    header_send_process(buf, &m->header);
    bias += 12;
    //query
    for (int i = 0; i < m->header.query_num; i++)
        bias = query_send_process(buf, bias, m->query + i);
    //answer
    for (int i = 0; i < m->header.answer_num; i++)
        bias = RR_send_process(buf, bias, m->answer + i);
    //authority
    int auth_cnt = 0; //可处理的authRR条目数（不对SOA做任何处理）
    for (int i = 0; i < m->header.authority_num; i++)
    {
        if (((m->authority + i)->RR_query.type == CNAME_RECORD || (m->authority + i)->RR_query.type == PTR_RECORD || (m->authority + i)->RR_query.type == NS_RECORD || (m->authority + i)->RR_query.type == A_RECORD || (m->authority + i)->RR_query.type == AAAA_RECORD))
        {
            bias = RR_send_process(buf, bias, m->authority + i);
            auth_cnt++;
        }
    }
    //addition
    int addi_cnt = 0;   //可处理的addiRR条目数（不对SOA做任何处理）
    for (int i = 0; i < m->header.addition_num; i++)
    {
        if (((m->addition + i)->RR_query.type == CNAME_RECORD || (m->addition + i)->RR_query.type == PTR_RECORD || (m->addition + i)->RR_query.type == NS_RECORD || (m->addition + i)->RR_query.type == A_RECORD || (m->addition + i)->RR_query.type == AAAA_RECORD))
        {
            bias = RR_send_process(buf, bias, m->addition + i);
            addi_cnt++;
        }
    }
    m->header.authority_num = auth_cnt; //重写包头
    m->header.addition_num = addi_cnt;
    header_send_process(buf, &m->header);
    return bias;
}

int resolve(struct message *rs_message, struct node *internal_cache, struct node *external_cache)
{
    static struct RR answer[MAX_DB_RETURN];
    int answer_num = 0;
    if (rs_message->header.QR == 0) //查询
    {
        if ((answer_num = query_in_database(internal_cache, rs_message->query, answer, 0)) && ((answer_num == -1) || answer[answer_num - 1].RR_query.type != CNAME_RECORD)) //内部cache命中且最终答案不是CNAME记录
        {
            //构造header
            rs_message->header.QR = 1;
            rs_message->header.RA = 1;
            if (answer_num != -1) //正常查询
            {
                debug_print("Record found in local cache, send it to client");
                rs_message->header.answer_num = answer_num;
            }
            else //域名被屏蔽
            {
                debug_print("Record found in local cache but it's BLOCKED, send it to client");
                answer_num = 0;
                rs_message->header.answer_num = 0;
                rs_message->header.rcode = 0b011;
            }
            //构造answer
            rs_message->answer = answer;
        }
        else if ((answer_num = query_in_database(external_cache, rs_message->query, answer, 0)) && answer[answer_num - 1].RR_query.type != CNAME_RECORD) //外部cache命中且最终答案不是CNAME记录，发送回应
        {
            debug_print("Record found in external cache, send it to client");
            //构造header
            rs_message->header.QR = 1;
            rs_message->header.RA = 1;
            rs_message->header.answer_num = answer_num;
            rs_message->answer = answer;
        }
        else
        {
            debug_print("Record NOT found in either cache, send request to external server");
            answer_num = -1; //特殊标识，向外部dns服务器发送请求
        }
    }
    else //响应
    {
        if (rs_message->header.rcode != NO_SUCH_NAME) //加入外部cache
        {
            debug_print("Record found in external server, save the record in cache and send it to client");
            for (int i = 0; i < rs_message->header.answer_num; i++)
            {
                char tmp[MAX_NAME_LEN];
                struct RR *now_answer = rs_message->answer + i;
                to_with_dot(tmp, now_answer->RR_query.name);
                struct node *new_node = add_node_in_tree(external_cache, tmp, strlen(tmp));
                add_node_data(new_node, now_answer->RR_query.type, now_answer->RD_data, now_answer->TTL);
            }
        }
        else
        {
            debug_print("Record NOT found in external server, send it to client");
        }
        answer_num = rs_message->header.answer_num;
    }
    return answer_num;
}