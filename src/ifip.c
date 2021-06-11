#include "ifip.h"
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

ifface_t *get_iface_byname(char *name) {
    ifface_t *iface = (ifface_t *)malloc(sizeof(ifface_t));
    int fd;
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(iface->ifname, name);
    strncpy(ifr.ifr_name, iface->ifname, IFNAMSIZ - 1);
    iface->addr = get_iface_attr(fd, &ifr, SIOCGIFADDR);
    iface->netmask = get_iface_attr(fd, &ifr, SIOCGIFNETMASK);
    iface->mac = get_iface_mac(fd, &ifr);
    //switch (ifr.ifr_hwaddr.sa_family) {
    //    case ARPHRD_ETHER:
    //        iface->type = "ether";
    //    case ARPHRD_LOOPBACK:
    //        iface->type = "local";
    //    default:
    //        iface->type = "unknown";
    //}
    if (ifr.ifr_hwaddr.sa_family == ARPHRD_ETHER)
        iface->type = "ether";
    else if (ifr.ifr_hwaddr.sa_family == ARPHRD_LOOPBACK)
        iface->type = "local";
    else
        iface->type = "unknown";
    close(fd);
    return iface;
}

char *get_local_ip() {
    int fd;
    char *addr = (char *)malloc(sizeof(char) * INET_ADDRSTRLEN);
    struct sockaddr_in loopback;

    fd = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&loopback, 0, sizeof(loopback));
    loopback.sin_family = AF_INET;
    loopback.sin_addr.s_addr = 1337;
    loopback.sin_port = htons(9);
    if (connect(fd, (struct sockaddr *)(&loopback), sizeof(loopback)) == -1) {
        close(fd);
        return NULL;
    }

    socklen_t addrlen = sizeof(loopback);
    if (getsockname(fd, (struct sockaddr *)(&loopback), &addrlen) == -1) {
        close(fd);
        printf("Could not getsockname\n");
        return NULL;
    }

    close(fd);

    if (inet_ntop(AF_INET, &loopback.sin_addr, addr, INET_ADDRSTRLEN) == 0x0) {
        return NULL;
    }
    return addr;
}

char *get_iface_attr(int fd, struct ifreq *ifr, int attr) {
    char *addr = (char *)malloc(sizeof(char) * 17);
    ioctl(fd, attr, ifr);
    strncpy(addr,
            (char *)inet_ntoa(((struct sockaddr_in *)&ifr->ifr_addr)->sin_addr),
            17);
    return addr;
}

char *get_iface_mac(int fd, struct ifreq *ifr) {
    unsigned char *mac = NULL;
    int i;
    char *macaddr = (char *)malloc(17 * sizeof(char));
    char sep[5];
    ioctl(fd, SIOCGIFHWADDR, ifr);
    mac = (unsigned char *)ifr->ifr_hwaddr.sa_data;
    sprintf(sep, "%.2x", mac[0]);
    strcpy(macaddr, sep);
    for (i = 1; i < 6; i++) {
        sprintf(sep, ":%.2x", mac[i]);
        strcat(macaddr, sep);
    }
    return macaddr;
}
