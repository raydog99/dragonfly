#ifndef NETDEV_H
#define NETDEV_H

#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "ethernet.h"
#include "utun.h"

struct netdev {
    uint32_t addr;
    unsigned char hwaddr[6];
    int utun_fd;
};

void netdev_init(struct netdev *dev, int utun_fd, char *addr, char *hwaddr);
void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, 
                     uint16_t ethertype, int len, unsigned char *dst);
#endif