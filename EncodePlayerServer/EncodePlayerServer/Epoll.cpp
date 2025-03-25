#include "Epoll.h"

CEpoll::CEpoll(int count) : m_iEpollSize(count) , m_epFD(-1)
{
}

void CEpoll::Create()
{
	m_epFD = epoll_create(m_iEpollSize);
}

void CEpoll::Add(int fd)
{
	epoll_event event;
	epoll_ctl(m_epFD, EPOLL_CTL_ADD, fd, &event);
}

void CEpoll::Modify(int fd)
{
	epoll_event event;
	epoll_ctl(m_epFD, EPOLL_CTL_MOD, fd, &event);
}

void CEpoll::Del(int fd)
{
	epoll_event event;
	epoll_ctl(m_epFD, EPOLL_CTL_DEL, fd, &event);
}

void CEpoll::WaitEvent(std::vector<epoll_event>& events)
{
	while (epoll_wait(m_epFD, events.data(), m_iEpollSize, 0))
	{
		for (size_t i = 0; i < events.size(); i++)
		{

		}
	}
}
