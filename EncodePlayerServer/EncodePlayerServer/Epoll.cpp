#include "Epoll.h"

CEpoll::CEpoll() : m_epFD(-1)
{
}

int CEpoll::Create(int count = 1024)
{
	m_iEpollSize = count;
	m_epFD = epoll_create(m_iEpollSize);
	if (m_epFD == -1) return -1;
	return 0;
}

int CEpoll::Add(int fd, const CEpollData& data, uint32_t event)
{
	epoll_event epEvent{ event,data };
	epoll_ctl(m_epFD, EPOLL_CTL_ADD, fd, &epEvent);
}

int CEpoll::Modify(int fd, const CEpollData& data, uint32_t event)
{
	if (m_epFD == -1) return -1;
	epoll_event epEvent{ event,data };
	int ret = epoll_ctl(m_epFD, EPOLL_CTL_MOD, fd, &epEvent);
	return ret;
}

int CEpoll::Del(int fd)
{
	if (m_epFD == -1) return -1;
	epoll_event event;
	int ret = epoll_ctl(m_epFD, EPOLL_CTL_DEL, fd, &event);
	return ret;
}

int CEpoll::WaitEvent(std::vector<epoll_event>& events)
{
	if (m_epFD == -1) return -1;
	std::vector<epoll_event> evs;
	int ret = epoll_wait(m_epFD, evs.data(), m_iEpollSize, 0);
	if (!ret) return -2;
	if (evs.size() > events.size())
	{
		events.resize(ret);
	}
	events.assign(evs.begin(), evs.end());
	return 0;
}
