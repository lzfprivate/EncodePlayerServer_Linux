#include "SocketLocal.h"
#include <fcntl.h>

CSocketLocal::CSocketLocal(CSocketParam param) : m_Param(param)
{

}

CSocketLocal::CSocketLocal(int fd)
{
    m_socket = fd;
}

int CSocketLocal::Initialize(const CSocketParam& param)
{
    if (m_iStatus != 0) return -1;          //如果已经初始化
    m_Param = param;
    int type = m_Param.m_iAttr & en_TCP ? SOCK_STREAM : SOCK_DGRAM; 
    m_socket = socket(AF_LOCAL, type, 0);
    if (m_socket == -1) return -2;          //通信句柄没有申请成功

    if (m_Param.m_iAttr & en_SERVER)        //当前端是服务端
    {
        int ret = bind(m_socket, m_Param.addrun(), sizeof(sockaddr_un));
        if (-1 == ret) return -3;           //绑定失败
        ret = listen(m_socket, 5);
        if (-1 == ret) return -4;           //监听失败
    }
    if (m_Param.m_iAttr & en_NONBLOCK)         //当前端设置非阻塞
    {
        int iFlag = fcntl(m_socket, F_GETFL);
        if (iFlag == -1) return -5;
        iFlag |= O_NONBLOCK;
        int ret = fcntl(m_socket, F_SETFL,iFlag);
        if (ret == -1) return -6;
    }
    m_iStatus = 1;
    return 0;
}

int CSocketLocal::Link(CSocketBase** socketClient)
{
    if (m_iStatus < 1 || m_socket == -1) return -1;
    int ret = -1;
    if (m_Param.m_iAttr & en_SERVER)    //当前端是服务端
    {
        if (*socketClient == nullptr)    return -2;
        CSocketParam param;
        socklen_t len = 0;
        int fd = accept(m_socket, param.addrun(), &len);
        if (fd == -1) return -3;
        *socketClient = new CSocketLocal(fd);
        if (*socketClient == nullptr) return -4;
        ret = (*socketClient)->Initialize(param);
        if (ret != 0)
        {
            delete* socketClient;
            socketClient = nullptr;
            return -5;
        }
    }
    else
    {
        ret = connect(m_socket, m_Param.addrun(), sizeof(sockaddr_un));
        if (ret == -1) return -6;
    }
    m_iStatus = 2;
    return 0;
}

int CSocketLocal::Send(const CCommonBuffer& buffer)
{
    if (m_iStatus < 2 || m_socket == -1) return -1;
    ssize_t index = 0;
    while (index < buffer.size())
    {
        ssize_t len = write(m_socket, buffer.data() + index, buffer.size() - index);
        if (len == 0) return -2;
        if (len < 0) return -3;
        index += len;
    }
    return 0;
}

int CSocketLocal::Recv(CCommonBuffer& buffer)
{
    if (m_iStatus < 2 || m_socket == -1) return -1;
    ssize_t len = read(m_socket, buffer, buffer.size());
    if (len > 0)
    {
        buffer.resize(len);
        return len;
    }
    if (len < 0)
    {
        if (errno == EINTR || errno == EAGAIN)
        {
            buffer.clear();
            return 0;
        }
        return -2;
    }
    return -3;
}
