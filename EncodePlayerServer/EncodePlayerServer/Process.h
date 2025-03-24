#pragma once
#include "FuntionProcess.h"

class CProcess
{
public:
	CProcess();
	virtual ~CProcess();

public:
	template<typename _FUNCTION_,typename... _ARGS_>
	bool SetFunctionEntry(_FUNCTION_ function, _ARGS_... args)
	{
		m_function = new CFuntionProcess<_FUNCTION_,_ARGS_...>(function,args...);
		return m_function;
	}

public:
	int CreateSubProcess();
	
	
private:
	CFunctionBase* m_function;
	
	pid_t			m_pid;

};

