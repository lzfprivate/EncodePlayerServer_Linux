#include "CLoggerServer.h"
#include <sys/stat.h>
#include <thread>
#include <sys/timeb.h>

using EPEVENTS = std::vector<epoll_event>;

CLoggerServer::CLoggerServer() 
	: m_thread(&CLoggerServer::ThreadFunction,this),m_server(nullptr)
{
	m_bufferPath = "./log/" + GetCurrTimeBuffer() + ".log";
	printf("[%s]<%d>(%s) %s", __FILE__, __LINE__, __FUNCTION__, (char*)m_bufferPath);
}

int CLoggerServer::Start()
{
	if (m_server) return -1;
	if (access("log", W_OK | R_OK) != 0)
	{
		mkdir("log", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	}
	m_file = fopen(m_bufferPath, "w+");
	if (m_file == nullptr) return -2;

	int ret = m_epoll.Create(1);
	if (0 != ret) return -3;
	m_server = new CSocketLocal();
	if (nullptr == m_server) return -4;
	ret = m_server->Initialize(CSocketParam("log/logger.sock",en_SERVER));
	if (0 != ret)
	{
		Close();
		return -5;
	}
	ret = m_thread.Start();
	if (0 != ret)
	{
		Close();
		return -6;
	}
	return 0;
}

int CLoggerServer::ThreadFunction()
{
	EPEVENTS evs;
	std::map<int, CSocketBase*> clients;
	while (m_thread.IsValid() && m_epoll != -1 && m_server)
	{
		int ret = m_epoll.WaitEvent(evs);
		if (ret < 0) break;
		if (ret > 0)
		{
			size_t i = 0;
			for (; i < ret; i++)
			{
				if (EPOLLERR == evs[i].events)
				{
					break;
				}
				else if(EPOLLIN == evs[i].events)
				{
					if (m_server == evs[i].data.ptr)
					{
						//连接，创建新套接字 
						CSocketBase* client = nullptr;
						int rst = m_server->Link(&client);
						if (rst < 0) continue;
						rst = m_epoll.Add(*client,CEpollData((void*)client), EPOLLIN | EPOLLERR);
						if (rst < 0)
						{
							delete client;
							continue;
						}
						auto iter = clients.find(*client);
						if (iter != clients.end())
						{
							delete iter->second;
						}
						clients[*client] = client;
					}
					else
					{
						CSocketBase* client = (CSocketBase*)evs[i].data.ptr;
						if (client != nullptr)
						{
							CCommonBuffer bufferRecv(1024 * 1024);
							int rst = client->Recv(bufferRecv);
							if (rst <= 0)
							{
								delete client;
								clients[*client] = nullptr;
								client = nullptr;
							}
							else
							{
								WriteLogger(bufferRecv);
							}
						}
					}
				}
			}
			if (i != ret)
			{
				break;
			}
		}
	}
	for (auto iter : clients)
	{
		if(iter.second)
			delete iter.second;
	}
	clients.clear();
	return 0;
}

void CLoggerServer::Close()
{
	if (m_server)
	{
		auto sock = m_server;
		m_server = nullptr;
		delete sock;
	}
	m_epoll.Close();
	m_thread.Stop();
}

void CLoggerServer::Trace(const LogInfo& info)
{
	static thread_local CSocketLocal client;
	if (client == -1)
	{
		int ret = client.Initialize(CSocketParam("./log/logger.sock",0));
		if (ret != 0)
		{
#ifdef DEBUG
			printf("[%s](%d)<%s> logger thread init ret:%d", __FILE__, __LINE__, __FUNCTION__, ret);
#endif // DEBUG
			return;
		}
	}
	client.Send(info.GetTotalBuffer());

}

CCommonBuffer CLoggerServer::GetCurrTimeBuffer()
{
	CCommonBuffer bufferTime;
	timeb tmb;
	ftime(&tmb);
	tm* ptm = localtime(&tmb.time);
	int nSize = snprintf(bufferTime, bufferTime.size(), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
		ptm->tm_year + 1900,ptm->tm_mon,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec,tmb.millitm);
	bufferTime.resize(nSize);
	return bufferTime;
}

void CLoggerServer::WriteLogger(const CCommonBuffer& buffer)
{
	if (m_file != nullptr)
	{
		FILE* pFile = m_file;
		fwrite((char*)buffer, 1, buffer.size(), pFile);
		fflush(pFile);
#ifdef DEBUG
		printf("[%s](%d)<%s> write content:%s", __FILE__, __LINE__, __FUNCTION__, buffer);
#endif // DEBUG

	}
}

#ifndef TRACE
#define TRACEI(...) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGINFO, __VA_ARGS__));
#define TRACED(...) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGDEBUG, __VA_ARGS__));
#define TRACEW(...) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGWARNING, __VA_ARGS__));
#define TRACEE(...) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGERROR, __VA_ARGS__));
#define TRACEF(...) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGFATAL, __VA_ARGS__));

#define LOGI() CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGINFO));
#define LOGD() CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGDEBUG));
#define LOGW() CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGWARNING));
#define LOGE() CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGERROR));
#define LOGF() CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGFATAL));

#define DUMPI(data,size) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGINFO, data,size));
#define DUMPD(data,size) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGDEBUG, data,size));
#define DUMPW(data,size) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGWARNING,data,size));
#define DUMPE(data,size) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGERROR, data,size));
#define DUMPF(data,size) CLoggerServer::Trace(LogInfo(__FILE__, __LINE__, __FUNCTION__, getpid(), pthread_self(), LOGFATAL,data,size));



#endif // !

