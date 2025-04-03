#pragma once
#include "EpollData.h"
#include <vector>

#define EVENT_MAX_SIZE 128
class CEpoll
{
public:
	CEpoll();
	CEpoll& operator=(const CEpoll& epoll) = delete;
	CEpoll(const CEpoll& epoll) = delete;
	~CEpoll();

	//创建
	int Create(int count = 1024);
	//添加
	int Add(int fd,const CEpollData& data = CEpollData((void*)0),uint32_t event = EPOLLIN);
	//修改
	int Modify(int fd, const CEpollData& data = CEpollData((void*)0), uint32_t event = EPOLLIN);
	//删除
	int Del(int fd);
	//等待
	int WaitEvent(std::vector<epoll_event>& events,int timeout = 10);
	//关闭
	void Close();

	operator int() const {
		return m_epFD;
	}


private:
	int m_iEpollSize;
	int m_epFD;
};


