#pragma once
#include <sys/epoll.h>

class CEpollData
{
public:
	CEpollData();
	CEpollData(const CEpollData& data);

	CEpollData& operator=(const CEpollData& data);

	CEpollData(void* ptr);
	CEpollData(int fd);
	CEpollData(uint32_t u32);
	CEpollData(uint64_t u64);

	operator void*();
	operator int();
	operator uint32_t();
	operator uint64_t();

private:
	epoll_data_t	m_data;

};

