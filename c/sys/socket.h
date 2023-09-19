#ifndef	_SOCKET_H
#define	_SOCKET_H

struct sockaddr{
	u_char sa_len;
	u_char sa_family;
	char sa_data[14];
};

struct osockaddr{
	u_short sa_family;
	char sa_data[14];
}

#endif