#include "Process.h"
#include <sys/types.h>
#include <sys/socket.h>

CProcess::CProcess() : m_function(nullptr),m_pid(-1)
{
}

CProcess::~CProcess()
{
}

int CProcess::CreateSubProcess()
{
	if (!m_function) return -1;

	pid_t pid = fork();
	if (pid == -1) return -2;

	if (pid == 0)
	{
		int ret = (*m_function)();
		return ret;
	}

	m_pid = pid;

    return 0;
}
