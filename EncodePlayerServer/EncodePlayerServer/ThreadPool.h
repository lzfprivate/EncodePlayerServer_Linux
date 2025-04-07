#pragma once
#include <queue>
#include "Epoll.h"
#include "CCommThread.h"
#include "SocketLocal.h"
#include <thread>

/*
* 本线程池是通过本地通信来分发任务
* 可以对本类修改，不使用通信方式来分发任务
*/

class CThreadPool
{
public:
	CThreadPool();
	~CThreadPool()
	{
		Close();
	}

	CThreadPool(const CThreadPool& pool) = delete;
	CThreadPool& operator=(const CThreadPool& pool) = delete;

public:
	//创建
	int Start(unsigned int size = 1024);
	//线程池大小
	size_t Size();
	//添加任务到任务队列
	template<typename _FUNCTION_,typename... _ARGS_>
	int AddTask(_FUNCTION_ function, _ARGS_... args)
	{
		int ret = -1;
		static thread_local CSocketLocal client;		
		if (client == -1)
		{
			ret = client.Initialize(CSocketParam(m_bufferPath,en_TCP));
			if (ret != 0) return -1;
			ret = client.Link();
			if (ret != 0) return -2;
		}
		CFunctionBase* function_ = new CFunctionThread<_FUNCTION_, _ARGS_...>(function, args...);
		if (function_ = nullptr) return -3;
		CCommonBuffer bufferSend(sizeof(function_));
		memcpy(bufferSend, &function_, sizeof(function_));
		ret = client.Send(bufferSend);
		if (ret != 0)
		{
			delete function_;
			return -4;
		}
		return 0;
	}

private:
	int DispatchTask();
	void Close();


	

private:
	CEpoll m_epoll;

	std::vector<CCommThread*>	m_vectorThread;					//任务阻塞队列

	CSocketBase* m_server;									//本地通信，用于发送任务函数	通用定义中的消费者线程

	CCommonBuffer m_bufferPath;								//本地通信路径

};

