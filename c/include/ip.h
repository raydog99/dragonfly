#ifndef _IP_H
#define _IP_H
#include <netinet/if_ether.h>

#define IPVERSION					4

#define IP_RF 0x8000                /* reserved fragment flag */
#define IP_DF 0x4000                /* dont fragment flag */
#define IP_MF 0x2000                /* more fragments flag */
#define IP_OFFMASK 0x1fff           /* mask for fragmenting bits */

struct iphdr {
	uint8_t version : IPVERSION;	/* version */
    uint8_t ihl : 4;				/* fixed at Little Endian byte order */
    uint8_t tos;					/* types of service */
    uint16_t len;					/* total length */
    uint16_t id;					/* identification */
    uint16_t flags : 3;				/* flags */
    uint16_t frag_offset : 13;		/* fragment offset field */
    uint8_t ttl;					/* time to live */
    uint8_t proto;					/* protocol */
    uint16_t csum;					/* checksum */
    uint32_t saddr;					/* source address */
    uint32_t daddr;					/* dst address */
}__attribute__((packed));

uint16_t checksum(void *addr, int count);

#endif