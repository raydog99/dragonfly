#include "tuntap_if.h"
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
  char dev[20];
  memset(&dev, 0, sizeof (dev));
  char ip_address[20] = "10.0.0.4";

  int fd = open_utun(dev, ip_address);
  if (fd <= 0){
    printf("Could not connect to utun devices, exiting\n");
    exit(0);
  }

  char mac_address[20];
  struct netdev netdev;
  netdev_init(&netdev, ip_address, mac_address);

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