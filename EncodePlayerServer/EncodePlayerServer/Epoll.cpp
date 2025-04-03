#include "Epoll.h"
#include <unistd.h>
#include <errno.h>
CEpoll::CEpoll() : m_epFD(-1)
{
}

CEpoll::~CEpoll()
{
	Close();
}

int CEpoll::Create(int count)
{
	if (m_epFD != -1) return -1;			//已经初始化
	m_iEpollSize = count;
	m_epFD = epoll_create(m_iEpollSize);
	if (m_epFD == -1) return -2;			//初始化失败
	return 0;
}

int CEpoll::Add(int fd, const CEpollData& data, uint32_t event)
{
	if (m_epFD == -1) return -1;
	epoll_event epEvent{ event,data };
	int ret = epoll_ctl(m_epFD, EPOLL_CTL_ADD, fd, &epEvent);
	if (-1 == ret) return -2;
	return 0;
}

int CEpoll::Modify(int fd, const CEpollData& data, uint32_t event)
{
	if (m_epFD == -1) return -1;
	epoll_event epEvent{ event,data };
	int ret = epoll_ctl(m_epFD, EPOLL_CTL_MOD, fd, &epEvent);
	if (-1 == ret) return -2;
	return 0;
}

int CEpoll::Del(int fd)
{
	if (m_epFD == -1) return -1;
	epoll_event event;
	int ret = epoll_ctl(m_epFD, EPOLL_CTL_DEL, fd, &event);
	if (-1 == ret) return -2;
	return 0;
}

int CEpoll::WaitEvent(std::vector<epoll_event>& events, int timeout)
{
	if (m_epFD == -1) return -1;
	std::vector<epoll_event> evs(EVENT_MAX_SIZE);
	int ret = epoll_wait(m_epFD, evs.data(), evs.size(), timeout);
	if (ret == -1)
	{
		if (errno == EINTR || errno == EAGAIN)
		{
			return 0;
		}
		return -2;
	}
	if (ret > events.size())
	{
		events.clear();
		events.resize(ret);
	}
	events.assign(evs.begin(), evs.end());
	return ret;
}

void CEpoll::Close()
{
	if (m_epFD != -1)
	{
		int fd = m_epFD;
		m_epFD = -1;
		close(fd);
	}
}
