#include "utun.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"

#define BUFLEN 100

void handle_frame(struct netdev *netdev, struct eth_hdr *hdr){
    switch (hdr->ethertype) {
        case ETHERTYPE_ARP:
            arp_incoming(netdev, hdr);
            break;
        case ETHERTYPE_IP:
            printf("Found IPv4\n");
            break;
        default:
            printf("Unrecognized ethertype %x\n", hdr->ethertype);
            break;
    }
}

int main(void) {
  char dev[100];
  memset(dev, 0, sizeof(dev));
  char ip_address[100] = "10.0.0.4";

  int fd = utun_init(dev, ip_address);
  if (fd <= 0){
    printf("Could not connect to utun devices, exiting\n");
    exit(0);
  }

  char mac_address[100] = "a0:78:17:70:a9:96";
  struct netdev netdev;
  netdev_init(&netdev, fd, ip_address, mac_address);

  arp_init();
  char buf[BUFLEN];

  while (1) {
      if (read(fd, buf, BUFLEN) < 0) {
          fprintf(stderr, "ERR: Read from tun_fd: %s\n", strerror(errno));
      }
      struct eth_hdr *hdr = init_eth_hdr(buf);
      handle_frame(&netdev, hdr);
    }
}