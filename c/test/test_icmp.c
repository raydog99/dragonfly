#include "utun.h"
#include "netdev.h"
#include "ip.h"

#define BUFLEN 100

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

  char buf[BUFLEN];

  while (1) {
      if (read(fd, buf, BUFLEN) < 0) {
          fprintf(stderr, "ERR: Read from tun_fd: %s\n", strerror(errno));
      }

      struct iphdr *ih = ip_hdr(buf);
      memset(buf, 0, sizeof(buf));
    }
}