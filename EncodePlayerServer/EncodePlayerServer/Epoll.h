#pragma once
#include "EpollData.h"
#include <vector>
class CEpoll
{
public:
	CEpoll();
	CEpoll& operator=(const CEpoll& epoll) = delete;
	CEpoll(const CEpoll& epoll) = delete;

	//创建
	int Create(int count = 1024);
	//添加
	int Add(int fd,const CEpollData& data = CEpollData((void*)0),uint32_t event = EPOLLIN);
	//修改
	int Modify(int fd, const CEpollData& data = CEpollData((void*)0), uint32_t event = EPOLLIN);
	//删除
	int Del(int fd);
	//等待
	int WaitEvent(std::vector<epoll_event>& events);


private:
	int m_iEpollSize;
	int m_epFD;
};


