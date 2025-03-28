#pragma once
#include <sys/epoll.h>

class CEpollData
{
public:
	CEpollData() {

	}
	CEpollData(const CEpollData& data) {
		m_data.u64 = data.m_data.u64;
	}
	CEpollData(void* ptr) {
		m_data.ptr = ptr;
	}
	CEpollData(int fd) {
		m_data.fd = fd;
	}
	CEpollData(uint32_t u32)
	{
		m_data.u32 = u32;
	}
	CEpollData(uint64_t u64)
	{
		m_data.u64 = u64;
	}

	CEpollData& operator=(const CEpollData& data) {
		if (this != &data)
		{
			m_data.u64 = data.m_data.u64;
		}
		return *this;
	}
	CEpollData& operator=(void* ptr) {
		return *this;
	}
	CEpollData& operator=(int fd) {
		return *this;
	}
	CEpollData& operator=(uint32_t u32) {
		return *this;
	}
	CEpollData& operator=(uint64_t u64) {
		return *this;
	}


	operator epoll_data_t() { return m_data; }
	operator epoll_data_t()const { return m_data; }
	operator epoll_data_t*() { return &m_data; }
	operator const epoll_data_t*() const { return &m_data; }

private:
	epoll_data_t	m_data;

};

