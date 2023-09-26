#ifndef UTUN_HELPER_H
#define UTUN_HELPER_H

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/kern_control.h>
#include <net/if.h>
#include <net/if_utun.h>
#include <sys/ioctl.h>
#include <sys/kern_event.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define UTUN_OPT_IFNAME 2
#define BUFLEN 100

int open_utun(char *dev);
int utun_read(char *buf, int len);
int utun_write(char *buf, int len);
static int set_if_up(char *dev, char *addr);
void utun_init(char *dev, char *address);

#endif