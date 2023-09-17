struct m_hdr{
	struct mbuf *mh_next;
	struct mbuf *mh_nextpkt;
	int * mh_len;
	chaddr_t mh_data;
	short mh_type;
	short mh_flags;
};

struct pkthdr{
	int len;
	struct ifnet *rcvif;
};

struct m_ext{
	caddr_t ext_buf;
	void (*ext_free) { };
	u_int ext_size;
};

struct mbuf{
	struct m_hdr m_hdr;
	union{
		struct{
			struct pkthdr MH_pkthdr;
			union {
				struct m_ext MH_ext;
				char MH_databuf[MHLEN];
			} MH_dat;
		} MH;
		char M_databuf[MLEN];
		} M_dat;
};

#define MGET(m, how, type)		((m) = m_get(how), (type))

#define m_next 		m_hdr.mh_next
#define m_len 		m_hdr.mh_len
#define m_data 		m_hdr.mh_data
#define m_type 		m_hdr.mh_type
#define m_flags 	m_hdr.mh_flags
#define m_nextpkt 	m_hdr.mh_nextpkt
#define m_act		m_nextpkt
#define m_pkthdr 	M_dat.MH.MH_pkthdr
#define m_ext 		M_dat.MH.MH_dat.MH_ext
#define m_pktdat 	M_dat.MH.MH.MH_databuf
#define m_dat 		M_dat.M_databuf