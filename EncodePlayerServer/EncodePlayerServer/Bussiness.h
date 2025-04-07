#pragma once
#include "FunctionThread.h"
class CBusiness
{
public:
	virtual int BusinessProcess() = 0;

	template<typename _FUNCTION_, typename... _ARGS_>
	bool SetConnectCallback(_FUNCTION_ function, _ARGS_... args)
	{
		m_functionConn = new CFunctionThread<_FUNCTION_, _ARGS_>(function, args...);
		return m_functionConn;
	}
	template<typename _FUNCTION_, typename... _ARGS_>
	bool SetReceiveCallback(_FUNCTION_ function, _ARGS_... args)
	{
		m_functionRecv = new CFunctionThread<_FUNCTION_, _ARGS_>(function, args...);
		return m_functionRecv;
	}

protected:
	CFunctionBase* m_functionConn;
	CFunctionBase* m_functionRecv;
};