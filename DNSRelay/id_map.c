#include "id_map.h"

struct id_map *new_list()
{
    struct id_map *head = malloc(sizeof(struct id_map));
    head->server_id = 0;
    head->next = NULL;
    return head;
}

void new_info(struct id_map *head, struct sockaddr_in cli_addr, u_short cli_id, u_short svr_id)
{
    while (head->next != NULL)
        head = head->next;
    struct id_map *new_map = malloc(sizeof(struct id_map));
    new_map->client_addr = cli_addr;
    new_map->client_id = cli_id;
    new_map->server_id = svr_id;
    new_map->set_up_time = time(NULL);
    new_map->next = NULL;
    head->next = new_map;
}

struct id_map *search_by_server_id(struct id_map *head, u_short server_id)
{
    while (head != NULL && head->server_id != server_id)
        head = head->next;
    return head;
}

void delete_by_server_id(struct id_map *head, u_short svr_id)
{
    while (head->next != NULL && head->next->server_id != svr_id)
        head = head->next;
    struct id_map *p = head->next;
    head->next = head->next->next;
    free(p);
}

void clear_outdated_info(struct id_map *head)
{
    while (head->next != NULL)
    {
        if (head->next->set_up_time + 10 < time(NULL))    //10秒过期
        {
            struct id_map *to_be_deleted = head->next;
            head->next = head->next->next;
            free(to_be_deleted);
        }
        else
            head = head->next;
    }
}