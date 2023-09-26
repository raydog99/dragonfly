#include "netdev.h"

void netdev_init(struct netdev *dev, int utun_fd, char *addr, char *hwaddr){
    memset(dev, 0, sizeof(struct netdev));
    if (inet_pton(AF_INET, addr, &dev->addr) != 1) {
        fprintf(stderr, "ERR: Parsing inet address failed %s\n", strerror(errno));
        exit(1);
    }
    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hwaddr[0],
                                                    &dev->hwaddr[1],
                                                    &dev->hwaddr[2],
                                                    &dev->hwaddr[3],
                                                    &dev->hwaddr[4],
                                                    &dev->hwaddr[5]);
    dev->utun_fd = utun_fd;
}

void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, 
                     uint16_t ethertype, int len, unsigned char *dst){
    hdr->ethertype = htons(ethertype);

    memcpy(hdr->smac, dev->hwaddr, 6);
    memcpy(hdr->dmac, dst, 6);
    len += sizeof(struct eth_hdr);
    utun_write(dev->utun_fd, (char *)hdr, len);
}