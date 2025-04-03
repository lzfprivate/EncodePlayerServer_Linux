#pragma once
#include <pthread.h>
#include <map>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "FunctionThread.h"
class CCommThread
{
public:
	CCommThread(const CCommThread& thread) = delete;
	CCommThread& operator=(const CCommThread& thread) = delete;
	~CCommThread();

	template <typename _FUNCTION_, typename ..._ARGS_>
	CCommThread(_FUNCTION_ function, _ARGS_... args) 
		: m_function(new CFunctionThread<_FUNCTION_, _ARGS_...>(function, args...))
	{
		m_pid = 0;
		m_bPause = false;
	}


	template <typename _FUNCTION_,typename ..._ARGS_>
	bool SetFunctionEntry(_FUNCTION_ function, _ARGS_... args)
	{
		m_function = new CFunctionThread<_FUNCTION_, _ARGS_...>(function, args...);
		return m_function;
	}

public:
	int Start();
	int Pause();
	int Stop();
	bool IsValid() const; 

private:
	static void* ThreadFunctionEntry(void* param);
	int ThreadMain();

	static void SigAction(int, siginfo_t*, void*);


protected:
	CFunctionBase* m_function;

private:
	pthread_t m_pid;
	bool m_bPause;

	static std::map<int, CCommThread*> m_mpThreads;
};

