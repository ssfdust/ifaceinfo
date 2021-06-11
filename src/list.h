#include "ifip.h"

typedef struct ifacelist {
  ifface_t *iface;
  struct ifacelist *next;
} ifacelist_t;


ifacelist_t *initifacelist();
int is_in_ifacelist(ifacelist_t *, char *);
int insert_ifacelist(ifacelist_t **, ifface_t*);
int len_ifacelist(ifacelist_t *);
void free_ifacelist(ifacelist_t **);
ifacelist_t * list_ifa();
