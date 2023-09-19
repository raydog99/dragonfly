#ifndef	_NET_IF_PRIVATE_H_
#define	_NET_IF_PRIVATE_H_

#ifdef	_KERNEL
struct ifnet {
	struct ifnet *if_next;
	struct ifaddr *if_addrlist;
	char *if_name;
	short if_unit;
	u_short if_index;
	short if_flags;
	short if_timer;
	int if_pcount;
	caddr if_bpf;

	int (*if_init) (int);
	int (*if_output) (struct ifnet*, struct mbuf*, struct sockaddr);
	int (*if_start) (struct ifnet*);
	int (*if_ioctl) (struct ifnet*, int, caddr_t);
	int (*if_reset) (int);
	int (*if_watchdog) (int);

	struct ifqueue{
		struct mbuf *ifq_head;
		struct mbuf *ifq_tail;
		int ifq_len;
		int ifq_maxlen;
		int ifq_drops;
	} if_snd;
};

struct ifaddr {
	struct ifaddr *ifa_next;
	struct ifnet *ifa_ifp;
	struct sockaddr *ifa_addr;
	struct sockaddr *ifa_dstaddr;
#define ifa_broadaddr ifa_dstaddr;
	struct sockaddr *ifa_netmask;
	void (*ifa_rtrequest) ();
	u_short (ifa_flags);
	short (ifa_refcnt);
	int (ifa_metric);
};

#endif
#endif