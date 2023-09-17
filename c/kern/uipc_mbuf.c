struct mbuf *
m_get(nowait, type)
int nowait, type;
{
	struct mbuf *m;

	MGET(m, nowait, type);
	return (m);
}