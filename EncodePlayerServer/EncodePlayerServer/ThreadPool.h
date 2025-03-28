#pragma once
#include <queue>
#include "CCommThread.h"
class CThreadPool
{
public:
	template<typename _FUNCTION_,typename... _ARGS_>
	void AddTask(_FUNCTION_ function, _ARGS_... args)
	{

	}



	

private:
	std::queue<CCommThread>	m_queue;

	CCommThread thread;						//消费者线程，从线程池中获取线程函数运行

};

