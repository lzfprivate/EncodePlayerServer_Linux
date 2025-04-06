#pragma once
#include "FuntionProcess.h"

class CProcess
{
public:
	CProcess();
	virtual ~CProcess();

public:
	//设置程序入口
	template<typename _FUNCTION_,typename... _ARGS_>
	bool SetFunctionEntry(_FUNCTION_ function, _ARGS_... args)
	{
		m_function = new CFuntionProcess<_FUNCTION_,_ARGS_...>(function,args...);
		return m_function != nullptr;
	}

public:
	//创建子进程
	int CreateSubProcess();
	//发送进程句柄
	int SendFD(int fd);
	//接收进程句柄
	int RecvFD(int& fd);
	
private:
	CFunctionBase* m_function;
	
	pid_t			m_pid;					//进程号

	int				m_pipes[2];				//通信管道

};

