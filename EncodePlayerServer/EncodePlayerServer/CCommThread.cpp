#include "CCommThread.h"

CCommThread::CCommThread()
{
}

void CCommThread::ThreadFunctionEntry(void* param)
{
	auto thiz = (CCommThread*)param;
	thiz->ThreadMain();
	pthread_exit(nullptr);
}

