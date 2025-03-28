#pragma once
#include <pthread.h>
#include "FunctionThread.h"
class CCommThread
{
public:
	CCommThread();
	CCommThread(const CCommThread& thread) = delete;
	CCommThread& operator=(const CCommThread& thread) = delete;


	template <typename _FUNCTION_,typename ..._ARGS_>
	int SetFunctionEntry(_FUNCTION_ function, _ARGS_... args)
	{
		m_function = new CFunctionThread(function, args...);
		return m_function;
	}

private:
	static void ThreadFunctionEntry(void* param);
	int ThreadMain();


protected:
	CFunctionBase* m_function;
};

