#include "Process.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

CProcess::CProcess() : m_function(nullptr),m_pid(-1)
{
	memset(m_pipes, 0, 2);
}

CProcess::~CProcess()
{
	if (m_function)
	{
		delete m_function;
	}
	m_function = nullptr;

}

int CProcess::CreateSubProcess()
{
	if (!m_function) return -1;

	int iRet = socketpair(AF_LOCAL, SOCK_STREAM, 0, m_pipes);
	if (iRet == -1) return -2;

	pid_t pid = fork();
	if (pid == -1) return -3;

	if (pid == 0)
	{
		close(m_pipes[1]);
		m_pipes[1] = 0;
		exit((*m_function)());
	}
	close(m_pipes[0]);
	m_pipes[0] = 0;
	m_pid = pid;

    return 0;
}

int CProcess::SendFD(int fd)
{
	msghdr hdr;
	iovec iov[2];

	char buffer[][10] = {
		"hello","hello"
	};

	iov[0].iov_base = buffer[0];
	iov[0].iov_len = strlen(buffer[0]);
	iov[1].iov_base = buffer[1];
	iov[1].iov_len = strlen(buffer[1]);
	hdr.msg_iov = iov;
	hdr.msg_iovlen = 2;

	cmsghdr* msg = (cmsghdr*)calloc(1,CMSG_LEN(sizeof(int)));
	if (!msg) return -1;
	msg->cmsg_type = SCM_RIGHTS;
	msg->cmsg_level = SOL_SOCKET;
	msg->cmsg_len = CMSG_LEN(sizeof(int));
	*(int*)CMSG_DATA(msg) = fd;


	hdr.msg_control = msg;
	hdr.msg_controllen = CMSG_LEN(sizeof(int));

	ssize_t ret = sendmsg(m_pipes[1], &hdr, 0);
	free(msg);
	if (-1 == ret)	return -2;
	return 0;
}

int CProcess::RecvFD(int& fd)
{
	msghdr hdr;
	iovec iov[2];

	iov[0].iov_base = (char*)"hello";
	iov[0].iov_len = 6;
	iov[0].iov_base = (char*)"hello";
	iov[1].iov_len = 6;
	hdr.msg_iov = iov;
	hdr.msg_iovlen = 2;

	cmsghdr* msg = (cmsghdr*)calloc(1, CMSG_LEN(sizeof(int)));
	if (!msg) return -1;
	msg->cmsg_type = SCM_RIGHTS;
	msg->cmsg_level = SOL_SOCKET;
	msg->cmsg_len = CMSG_LEN(sizeof(int));

	hdr.msg_control = msg;
	hdr.msg_controllen = CMSG_LEN(sizeof(int));

	ssize_t ret = recvmsg(m_pipes[0], &hdr, 0);
	
	if (-1 == ret) {
		free(msg);
		return -2;
	}
	fd = *(int*)CMSG_DATA(msg);
	free(msg);
	return 0;
}
