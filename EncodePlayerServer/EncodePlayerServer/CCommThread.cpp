#include "CCommThread.h"


std::map<int, CCommThread*> CCommThread::m_mpThreads;


CCommThread::~CCommThread()
{
}

int CCommThread::Start()
{
	pthread_attr_t attr;
	int ret = pthread_attr_init(&attr);
	if (ret != 0) return -1;
	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (ret != 0) return -2;
	ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
	if (ret != 0) return -3;

	ret = pthread_create(&m_pid, &attr, &CCommThread::ThreadFunctionEntry, this);
	if (ret != 0) return -4;
	m_mpThreads[m_pid] = this;
	ret = pthread_attr_destroy(&attr);
	if (ret != 0) return -5;
	return 0;
}

int CCommThread::Pause()
{
	if (m_pid == -1) return -1;
	if (m_bPause)
	{
		m_bPause = false;
		return 0;
	}
	m_bPause = true;
	int ret = pthread_kill(m_pid, SIGUSR1);
	if (!ret) 
	{
		m_bPause = false;
		return -2;
	}
	return 0;
}

int CCommThread::Stop()
{
	if (m_pid != 0)
	{
		pthread_t thread = m_pid;
		m_pid = 0;
		timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 100 * 1000000;
		int ret = pthread_timedjoin_np(thread, nullptr, &ts);
		if (ret == ETIMEDOUT)
		{
			pthread_detach(thread);
			pthread_kill(thread, SIGUSR2);
		}
	}
	return 0;
}

bool CCommThread::IsValid() const
{
	return !(m_pid == 0);
}

void* CCommThread::ThreadFunctionEntry(void* param)
{
	auto thiz = (CCommThread*)param;
	struct sigaction act = { 0 };
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &CCommThread::SigAction;
	sigaction(SIGUSR1, &act, nullptr);
	sigaction(SIGUSR2, &act, nullptr);

	thiz->ThreadMain();

	if (thiz->m_pid) thiz->m_pid = 0;
	pthread_t thread = pthread_self();
	auto it = m_mpThreads.find(thread);
	if (it != m_mpThreads.end())
		it->second = nullptr;

	pthread_detach(thread);
	pthread_exit(nullptr);
}

int CCommThread::ThreadMain()
{
	if (m_function)
	{
		int ret = (*m_function)();
		if (ret != 0)
		{
			printf("[%s](%d)<%s> errno:%d,errmsg:%s\n", __FILE__, __LINE__, __FUNCTION__, errno, strerror(errno));
		}
	}
	return 0;
}

void CCommThread::SigAction(int signo, siginfo_t* info, void* context)
{
	if (signo == SIGUSR1)
	{
		pthread_t thread = pthread_self();
		auto it = m_mpThreads.find(thread);
		if (it != m_mpThreads.end())
		{
			if (it->second)
			{
				while (it->second->m_pid == 0)
				{
					pthread_exit(nullptr);
				}
				usleep(1000);
			}
		}
	}
	else if (signo == SIGUSR2)
	{
		pthread_exit(nullptr);
	}
}

