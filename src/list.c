#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>

ifacelist_t *initifacelist() {
    ifacelist_t *list = NULL;
    list = (ifacelist_t *)malloc(sizeof(ifacelist_t));
    if (list == NULL) {
        printf("[initifacelist_t]malloc list failed!\n");
        return NULL;
    }
    list->next = NULL;
    return list;
}

int is_in_ifacelist(ifacelist_t *list, char *ifname) {
    if (list == NULL)
        return -1;
    while (list->next) {
        if (strcmp(list->next->iface->ifname, ifname) == 0) {
            return 1;
        }
        list = list->next;
    }
    return 0;
}

int insert_ifacelist(ifacelist_t **list, ifface_t *iface) {
    ifacelist_t *current = NULL;
    ifacelist_t *head = *list;
    current = initifacelist();
    if (current == NULL)
        return -1;
    current->iface = iface;
    while (head->next)
        head = head->next;
    if (head->next == NULL)
        head->next = current;
    return 0;
}

int len_ifacelist(ifacelist_t* head)
{
    int len = 0;
    while(head && head->next)
    {
        len++;
        head = head->next;
    }
    return len;
}

void free_ifacelist(ifacelist_t** list)
{
    ifacelist_t* head = *list;
    ifacelist_t* p = NULL;
    while(head)
    { 
        p = head;
        head = head->next;
        free((void *)p);
    }
    return;
}

ifacelist_t* list_ifa() {
    struct ifaddrs *ifaddr, *ifa;
    char *ifnames[100];
    int i;
    ifacelist_t *head = initifacelist();

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return NULL;
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        char protocol[IFNAMSIZ] = {0};
        ifface_t *iface = get_iface_byname(ifa->ifa_name);
        if (!is_in_ifacelist(head, iface->ifname))
            insert_ifacelist(&head, iface);

        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET)
            continue;
    }

    freeifaddrs(ifaddr);
    return head;
}
