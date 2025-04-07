#include "ThreadPool.h"


CThreadPool::CThreadPool() : m_server(nullptr)
{
	timespec ts{ 0,0 };
	clock_gettime(CLOCK_REALTIME, &ts);
	char* buf = nullptr;
	asprintf(&buf, "%d_%d.sock", ts.tv_sec % 100000, ts.tv_nsec % 1000000);
	if (buf)
	{
		m_bufferPath = buf;
		free(buf);
	}
	usleep(1);
}

int CThreadPool::Start(unsigned int size)
{
	if (m_server != nullptr) return -1;
	if (m_bufferPath.size() == 0) return -2;
	m_server = new CSocketLocal();
	if (m_server == nullptr) return -3;
	int ret = m_server->Initialize(CSocketParam(m_bufferPath, en_SERVER));
	if (ret != 0) return -4;
	ret = m_epoll.Create(size);
	if (ret != 0) return -5;
	ret = m_epoll.Add(*m_server, CEpollData(m_server), EPOLLIN);
	if (ret != 0) return -6;
	m_vectorThread.resize(size);
	for (size_t i = 0; i < size; i++)
	{
		m_vectorThread[i] = new CCommThread(&CThreadPool::DispatchTask, this);
		if (m_vectorThread[i] = nullptr) return -7;
		ret = m_vectorThread[i]->Start();
		if (ret != 0) return -8;
	}
	return 0;
}

size_t CThreadPool::Size()
{
	return m_vectorThread.size();
}

int CThreadPool::DispatchTask()
{
	while (m_epoll != -1)
	{
		std::vector<epoll_event> evs;
		int ret = 0;
		ssize_t esize = m_epoll.WaitEvent(evs);
		if (esize > 0)
		{
			for (size_t i = 0; i < esize; i++)
			{
				if (evs[i].events & EPOLLIN)
				{
					CSocketBase* client = nullptr;
					if (evs[i].data.ptr == m_server)
					{
						//收到客户端连接
						ret = m_server->Link(&client);
						if (ret != 0) continue;
						ret = m_epoll.Add(*client, CEpollData(client));
						if (ret != 0) {
							delete client;
							continue;
						}
					}
					else
					{
						//收到客户端数据
						client = (CSocketLocal*)evs[i].data.ptr;
						CFunctionBase* function = nullptr;
						CCommonBuffer bufferRecv(sizeof(function));
						ret = client->Recv(bufferRecv);
						if (ret != 0)
						{
							m_epoll.Del(*client);
							delete client;
							continue;
						}
						memcpy(&function, bufferRecv.c_str(), sizeof(function));
						if (function)
						{
							(*function)();
							delete function;
						}
					}
				}
			}
		}
	}
}

void CThreadPool::Close()
{
	m_epoll.Close();
	if (m_server)
	{
		CSocketBase* sock = m_server;
		m_server = nullptr;
		delete sock;
	}
	for (auto iter : m_vectorThread)
	{
		if (iter) delete iter;
	}
	m_vectorThread.clear();
	unlink(m_bufferPath);
}
