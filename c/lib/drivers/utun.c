#include "utun.h"

int open_utun(char *dev) {
  int err;
  int fd;
  fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
  if (fd < 0) {
    return fd;
  }

  struct sockaddr_ctl addr;
  struct ctl_info info;
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

  char ifname[20];
  socklen_t ifname_len = sizeof(ifname);
  err = getsockopt(fd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME, ifname, &ifname_len);
  if (err < 0) {
    fprintf(stderr, "Error getting socket opt: %s\n", strerror (errno));
    return err;
  }

  strcpy(dev, ifname);
  return fd;
}

int utun_read(int utun_fd, char *buf, int len){
  return read(utun_fd, buf, len);
}

int utun_write(int utun_fd, char *buf, int len){
  return write(utun_fd, buf, len);
}

static int set_if_up(int utun_fd, char *dev, char *addr){
  char command[100];
  snprintf(command, sizeof(command), "ifconfig %s %s %s netmask 255.255.255.0 up", dev, addr, addr);
  return system(command);
}

int utun_init(char *dev, char *addr){
  int utun_fd = open_utun(dev);
  if (set_if_up(utun_fd, dev, addr) != 0){
    fprintf(stderr, "Error setting up interface %s\n", strerror(errno));
  }
  return utun_fd;
}