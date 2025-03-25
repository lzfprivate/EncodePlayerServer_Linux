#pragma once
#include "EpollData.h"
#include <vector>
class CEpoll
{
public:
	CEpoll(int count = 1024);
	CEpoll& operator=(const CEpoll& epoll) = delete;
	CEpoll(const CEpoll& epoll) = delete;

	//创建
	void Create();
	//添加
	void Add(int fd);
	//修改
	void Modify(int fd);
	//删除
	void Del(int fd);
	//等待
	void WaitEvent();


private:
	int m_iEpollSize;

	int m_epFD;

	epoll_event					m_epEvent;
	std::vector<epoll_event>	m_events;
	

	CEpollData	m_Data;
};

