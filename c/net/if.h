#ifndef _NET_IF_H_
#define	_NET_IF_H_

#define	IFF_UP		0x1		/* (n) interface is up */
#define	IFF_BROADCAST	0x2		/* (i) broadcast address valid */
#define	IFF_DEBUG	0x4		/* (n) turn on debugging */
#define	IFF_LOOPBACK	0x8		/* (i) is a loopback net */
#define	IFF_POINTOPOINT	0x10		/* (i) is a point-to-point link */
#define	IFF_NEEDSEPOCH	0x20		/* (i) calls if_input w/o net epoch */
#define	IFF_DRV_RUNNING	0x40		/* (d) resources allocated */
#define	IFF_NOARP	0x80		/* (n) no address resolution protocol */
#define	IFF_PROMISC	0x100		/* (n) receive all packets */
#define	IFF_ALLMULTI	0x200		/* (n) receive all multicast packets */
#define	IFF_DRV_OACTIVE	0x400		/* (d) tx hardware queue is full */
#define	IFF_SIMPLEX	0x800		/* (i) can't hear own transmissions */
#define	IFF_LINK0	0x1000		/* per link layer defined bit */
#define	IFF_LINK1	0x2000		/* per link layer defined bit */
#define	IFF_LINK2	0x4000		/* per link layer defined bit */
#define	IFF_ALTPHYS	IFF_LINK2	/* use alternate physical connection */
#define	IFF_MULTICAST	0x8000		/* (i) supports multicast */
#define	IFF_CANTCONFIG	0x10000		/* (i) unconfigurable using ioctl(2) */
#define	IFF_PPROMISC	0x20000		/* (n) user-requested promisc mode */
#define	IFF_MONITOR	0x40000		/* (n) user-requested monitor mode */
#define	IFF_STATICARP	0x80000		/* (n) static ARP */
#define	IFF_STICKYARP	0x100000	/* (n) sticky ARP */
#define	IFF_DYING	0x200000	/* (n) interface is winding down */
#define	IFF_RENAMING	0x400000	/* (n) interface is being renamed */
#define	IFF_SPARE	0x800000
#define	IFF_NETLINK_1	0x1000000	/* (n) used by netlink */

struct if_data {
	uint8_t	ifi_type;		/* ethernet, tokenring, etc */
	uint8_t	ifi_physical;		/* e.g., AUI, Thinnet, 10base-T, etc */
	uint8_t	ifi_addrlen;		/* media address length */
	uint8_t	ifi_hdrlen;		/* media header length */
	uint8_t	ifi_link_state;		/* current link state */
	uint8_t	ifi_vhid;		/* carp vhid */
	uint16_t	ifi_datalen;	/* length of this data struct */
	uint32_t	ifi_mtu;	/* maximum transmission unit */
	uint32_t	ifi_metric;	/* routing metric (external only) */
	uint64_t	ifi_baudrate;	/* linespeed */
	/* volatile statistics */
	uint64_t	ifi_ipackets;	/* packets received on interface */
	uint64_t	ifi_ierrors;	/* input errors on interface */
	uint64_t	ifi_opackets;	/* packets sent on interface */
	uint64_t	ifi_oerrors;	/* output errors on interface */
	uint64_t	ifi_collisions;	/* collisions on csma interfaces */
	uint64_t	ifi_ibytes;	/* total number of octets received */
	uint64_t	ifi_obytes;	/* total number of octets sent */
	uint64_t	ifi_imcasts;	/* packets received via multicast */
	uint64_t	ifi_omcasts;	/* packets sent via multicast */
	uint64_t	ifi_iqdrops;	/* dropped on input */
	uint64_t	ifi_oqdrops;	/* dropped on output */
	uint64_t	ifi_noproto;	/* destined for unsupported protocol */
	uint64_t	ifi_hwassist;	/* HW offload capabilities, see IFCAP */

	union {
		time_t		tt;
		uint64_t	ph;
	} __ifi_epoch;
#define	ifi_epoch	__ifi_epoch.tt
	union {
		struct timeval	tv;
		struct {
			uint64_t ph1;
			uint64_t ph2;
		} ph;
	} __ifi_lastchange;
#define	ifi_lastchange	__ifi_lastchange.tv
};


#endif