#pragma once
#include "Bussiness.h"
#include "Epoll.h"
#include "ThreadPool.h"
#include "CommonBuffer.h"
#include "Process.h"
class CServer
{
public:
	CServer();
	~CServer();
	CServer(const CServer& server) = delete;
	CServer& operator=(const CServer& server) = delete;

	int Initialize(CBusiness *bussiness,const CCommonBuffer bufferIP = "127.0.0.1",short sPort = 9999);
	int Run();
	int Close();


private:
	int ThreadFunction();

private:
	CEpoll	m_epoll;
	CThreadPool m_pool;
	CSocketBase* m_server;
	CProcess* m_process;
	CBusiness* m_bussiness;
};

