#include "CServer.h"
#include "SocketNetwork.h"
#include "CLoggerServer.h"

CServer::CServer():m_server(nullptr),m_bussiness(nullptr)
{
}

CServer::~CServer()
{
}

int CServer::Initialize(CBusiness* bussiness, const CCommonBuffer bufferIP, short sPort)
{
    int ret = 0;
    if (bussiness == nullptr) return -1;
    m_bussiness = bussiness;
    bool bTrue = m_process->SetFunctionEntry(&CBusiness::BusinessProcess, m_bussiness);
    if (bTrue) return -2;
    ret = m_process->CreateSubProcess();
    if (ret != 0) return -3;
    ret = m_pool.Start(2);
    if (ret != 0) return -4;
    ret = m_epoll.Create(2);
    if (ret != 0) return -5;
    m_server = new CSocketNetwork();
    if (m_server == nullptr) return -6;
    ret = m_server->Initialize(CSocketParam(bufferIP, sPort, en_SERVER | en_TCP | en_NETWORK));
    if (ret != 0) return -7;
    ret = m_epoll.Add(*m_server, CEpollData(m_server));
    if (ret != 0) return -8;
    for (size_t i = 0; i < m_pool.Size(); i++)
    {
        ret = m_pool.AddTask(&CServer::ThreadFunction, this);
        if (ret != 0) return -9;
    }
    return 0;
}

int CServer::Run()
{
    while (m_server)
    {
        usleep(10);
    }
    return 0;
}

int CServer::Close()
{
    return 0;
}

int CServer::ThreadFunction()
{
    int ret = 0;
    std::vector<epoll_event> evs;
    while (m_server && m_epoll != -1)
    {
        ssize_t size = m_epoll.WaitEvent(evs);
        if (size < 0) break;
        if (size > 0)
        {
            for (size_t i = 0; i < size; i++)
            {
                if (evs[i].events & EPOLLERR) break;
                else if (evs[i].events & EPOLLIN)
                {
                    if (m_server)
                    {
                        CSocketBase* client = NULL;
                        ret = m_server->Link(&client);
                        if (ret != 0)
                        {
                            if (client) delete client;
                            continue;
                        }
                        ret = m_process->SendFD(*client);
                        delete client;
                        if (ret != 0)
                        {
                            //TODO:¥Ú”°»’÷æ
                            continue;
                        }

                    }
                }
            }
        }
    }
    return 0;
}
