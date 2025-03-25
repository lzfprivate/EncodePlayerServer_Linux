#include "EpollData.h"

CEpollData::CEpollData()
{
}

CEpollData::CEpollData(const CEpollData& data)
{
}

CEpollData& CEpollData::operator=(const CEpollData& data)
{
	// TODO: 在此处插入 return 语句
	if (&data != this)
	{
		m_data.u64 = data.m_data.u64;
	}
	return *this;
}

CEpollData::CEpollData(void* ptr)
{
	m_data.ptr = ptr;
}

CEpollData::CEpollData(int fd)
{
	m_data.fd = fd;
}

CEpollData::CEpollData(uint32_t u32)
{
	m_data.u32 = u32;
}

CEpollData::CEpollData(uint64_t u64)
{
	m_data.u64 = u64;
}

CEpollData::operator void* ()
{
	return m_data.ptr;
}

CEpollData::operator int()
{
	return m_data.fd;
}

CEpollData::operator uint32_t()
{
	return m_data.u32;
}

CEpollData::operator uint64_t()
{
	return m_data.u64;
}
