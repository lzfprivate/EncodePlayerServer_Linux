#pragma once
#include "LogInfo.h"
#include "CCommThread.h"
#include "SocketLocal.h"
#include "Epoll.h"

class CLoggerServer
{
public:
	CLoggerServer();
	CLoggerServer& operator=(const CLoggerServer&) = delete;
	CLoggerServer(const CLoggerServer&) = delete;
	~CLoggerServer()
	{
		Close();
	}

public:
	int Start();

	int ThreadFunction();

	void Close();

	static void Trace(const LogInfo& info);

	static CCommonBuffer GetCurrTimeBuffer();

private:
	void WriteLogger(const CCommonBuffer& buffer);

private:
	CCommThread m_thread;			//主线程
	CEpoll		m_epoll;			//epoll用于客户端连入
	CSocketBase* m_server;			//通信
	CCommonBuffer m_bufferPath;		//本地通信文件		
	FILE* m_file;					//文件写入

};




