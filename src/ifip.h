#include <arpa/inet.h>
#include <net/if.h>

typedef struct ifface {
  char ifname[20];
  char *netmask;
  char *addr;
  char *mac;
  char *type;
} ifface_t;

char *get_iface_attr(int, struct ifreq *, int);
char *get_iface_mac(int, struct ifreq *);
char *get_local_ip(void);
ifface_t *get_iface_byname(char *);
