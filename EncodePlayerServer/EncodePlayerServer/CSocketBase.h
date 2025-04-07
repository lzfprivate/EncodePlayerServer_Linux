#pragma once
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "CommonBuffer.h"

enum class SockAttr
{
	SOCK_SERVER = 1,
	SOCK_TCP = 2,
	SOCK_NONBLOCK = 4
};

class CSocketParam
{
public:
	CSocketParam() {
		bzero(&m_addrin, sizeof(sockaddr_in));
		bzero(&m_addrun, sizeof(sockaddr_un));
		m_nPort = -1;
		m_iAttr = 0;
	}
	~CSocketParam() {

	}

	CSocketParam(const CSocketParam& param) {
		m_bufferIP = param.m_bufferIP;
		m_nPort = param.m_nPort;
		m_iAttr = param.m_iAttr;
		memcpy(&m_addrin, &param.m_addrin, sizeof(sockaddr_in));
		memcpy(&m_addrun, &param.m_addrun, sizeof(sockaddr_un));
	}

	CSocketParam(const CCommonBuffer& strIp, short nPort, int attr)
	{
		m_bufferIP = strIp;
		m_nPort = nPort;
		m_iAttr = attr;
		m_addrin.sin_family = AF_INET;
		m_addrin.sin_port = nPort;
		m_addrin.sin_addr.s_addr = inet_addr(strIp.c_str());
	}

	CSocketParam(const CCommonBuffer& path, int attr)
	{
		m_addrun.sun_family = AF_UNIX;
		strcpy(m_addrun.sun_path, path.c_str());
		m_iAttr = attr;
	}

	

	CSocketParam& operator=(const CSocketParam& param) {
		if(&param != this)
		{
			m_bufferIP = param.m_bufferIP;
			m_nPort = param.m_nPort;
			m_iAttr = param.m_iAttr;
			memcpy(&m_addrin, &param.m_addrin, sizeof(sockaddr_in));
			memcpy(&m_addrun, &param.m_addrun, sizeof(sockaddr_un));
		}
		return *this;
	}

	sockaddr* addrin() const {
		return (sockaddr*)&m_addrin;
	}

	sockaddr* addrun() const {
		return (sockaddr*)&m_addrun;
	}

public:
	CCommonBuffer	m_bufferIP;			//如果是本地通信 是文本字符串 如果是网络通信 是ip字符串
	short			m_nPort;			//端口

	int				m_iAttr;			//属性

	sockaddr_in		m_addrin;			//网络通信
	sockaddr_un		m_addrun;			//本地通信




};

class CSocketBase
{
public:
	virtual ~CSocketBase()
	{
		Close();
	}
	//初始化 服务端：创建、绑定、监听	客户端：创建
	virtual int Initialize(const CSocketParam& param) = 0;
	//连接	服务端：接收客户端连接		客户端：主动连接服务端
	virtual int Link(CSocketBase** client = nullptr) = 0;
	//发送数据
	virtual int Send(const CCommonBuffer& buffer) = 0;
	//接收数据
	virtual int Recv(CCommonBuffer& buffer) = 0;
	void Close() {
		m_iStatus = 3;
		if (m_socket != -1)
		{
			int fd = m_socket;
			m_socket = -1;
			close(fd);
		}
	}

	operator int() const{
		return m_socket;
	}

protected:
	int		m_socket;			//通信句柄
	int		m_iStatus;			//状态		0：未初始化 1：初始化完成 2：连接完成 3：关闭

	CSocketParam m_Param;		//通信参数

};

