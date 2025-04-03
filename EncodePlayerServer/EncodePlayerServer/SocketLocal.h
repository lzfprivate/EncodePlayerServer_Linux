#pragma once
#include "CSocketBase.h"
enum enSockParam
{
	en_NONE = 0,
	en_SERVER = 1,
	en_TCP = 2,
	en_NONBLOCK = 4
};

class CSocketLocal :
    public CSocketBase
{
public:
	CSocketLocal(CSocketParam param);
	CSocketLocal(int fd);
	CSocketLocal& operator=(const CSocketLocal& socket) = delete;
	CSocketLocal(const CSocketLocal& socket) = delete;

public:
	virtual int Initialize(const CSocketParam& param) ;
	virtual int Link(CSocketBase** socketClient);
	virtual int Send(const CCommonBuffer& buffer);
	virtual int Recv(CCommonBuffer& buffer);
	void Close() {
		CSocketBase::Close();
	}

protected:
	CSocketParam m_Param;
};

