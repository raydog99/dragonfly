#include "utun.h"

int open_utun(char *dev) {
  int err;
  int fd;
  char ifname[20];
  socklen_t ifname_len = sizeof(ifname);
  struct ifreq ifr;
  struct sockaddr_ctl addr;
  struct ctl_info info;

  fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
  if (fd < 0) {
    return fd;
  }
  memset(&info, 0, sizeof (info));
  strncpy(info.ctl_name, UTUN_CONTROL_NAME, strlen(UTUN_CONTROL_NAME));
  err = ioctl(fd, CTLIOCGINFO, &info);
  if (err < 0) {
    close(fd);
    return err;
  }

  addr.sc_id = info.ctl_id;
  addr.sc_len = sizeof(addr);
  addr.sc_family = AF_SYSTEM;
  addr.ss_sysaddr = AF_SYS_CONTROL;
  addr.sc_unit = 0;

  err = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
  if (err < 0) {
    fprintf(stderr, "Error connecting: %s\n", strerror( errno ));
    close(fd);
    return err;
  }

  err = getsockopt(fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, ifname, &ifname_len);
  if (err < 0) {
    fprintf(stderr, "Error getting socket opt: %s\n", strerror (errno));
    return err;
  }

  strcpy(dev, ifname);
  return fd;
}

int utun_read(char *buf, int len){
  return read(utun_fd, buf, len);
}

int utun_write(char *buf, int len){
  return write(utun_fd, buf, len);
}

static int set_if_up(char *dev, char *addr){
  return system("ifconfig %s %s %s netmask 255.255.255.0 up", dev, dev, addr);
}

void utun_init(char *dev, char *address){
  int utun_fd = utun_alloc(dev);

  if (set_if_up(dev) != 0){
    fprintf(stderr, "Error setting up interface\n", strerror(errno));
  }
}