#ifndef _NETINET_IP_ICMP_H_
#define _NETINET_IP_ICMP_H_

struct icmp_v4 {
	uint8_t type;               /* type of message */
    uint8_t code;               /* type sub code */
    uint16_t csum;              /* ones complement cksum of struct */
    uint8_t data[];             /* packet payload */
} __attribute__((packed));

void icmpv4_incoming(struct sk_buff *skb);
void icmpv4_reply(struct sk_buff *skb);

#endif