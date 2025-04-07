#pragma once
#include "CSocketBase.h"
class CSocketNetwork :
    public CSocketBase
{
public:
	CSocketNetwork();
	CSocketNetwork(CSocketParam param);
	CSocketNetwork(int fd);

public:
	virtual int Initialize(const CSocketParam& param);
	virtual int Link(CSocketBase** client = nullptr);
	virtual int Send(const CCommonBuffer& buffer);
	virtual int Recv(CCommonBuffer& buffer);

private:
	CSocketParam	m_Param;

};

