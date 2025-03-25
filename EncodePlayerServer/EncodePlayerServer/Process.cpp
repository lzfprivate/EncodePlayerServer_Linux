#include "Process.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

CProcess::CProcess() : m_function(nullptr),m_pid(-1)
{
}

CProcess::~CProcess()
{
}

int CProcess::CreateSubProcess()
{
	if (!m_function) return -1;

	int fd = socketpair(AF_LOCAL, SOCK_STREAM, 0, m_pipes);
	if (fd == -1) return -2;

	pid_t pid = fork();
	if (pid == -1) return -3;

	if (pid == 0)
	{
		m_pipes[0] = 0;
		close(m_pipes[0]);
		int ret = (*m_function)();
		return ret;
	}

	m_pipes[1] = 0;
	close(m_pipes[1]);
	m_pid = pid;

    return 0;
}

int CProcess::SendFD(int fd)
{
	msghdr hdr;
	iovec iov[2];

	memcpy(iov[0].iov_base, "hello", 6);
	iov[0].iov_len = 6;
	memcpy(iov[1].iov_base, "hello", 6);
	iov[1].iov_len = 6;
	hdr.msg_iov = iov;
	hdr.msg_iovlen = 2;

	cmsghdr* msg = new cmsghdr();
	msg->cmsg_type = SOL_SOCKET;
	msg->cmsg_len = CMSG_LEN(sizeof(int));
	*CMSG_DATA(msg) = fd;


	hdr.msg_control = msg;
	hdr.msg_controllen = CMSG_LEN(sizeof(int));

	return sendmsg(fd, &hdr, 0);
}

int CProcess::RecvFD(int& fd)
{
	msghdr hdr;
	iovec iov[2];
	memset(iov, 0, sizeof(iov));

	memcpy(iov[0].iov_base, "hello", 6);
	iov[0].iov_len = 6;
	memcpy(iov[1].iov_base, "hello", 6);
	iov[1].iov_len = 6;
	hdr.msg_iov = iov;
	hdr.msg_iovlen = 2;

	cmsghdr* msg = new cmsghdr();
	msg->cmsg_type = SOL_SOCKET;
	msg->cmsg_len = CMSG_LEN(sizeof(int));
	fd = *CMSG_DATA(msg);
	hdr.msg_control = msg;
	hdr.msg_controllen = CMSG_LEN(sizeof(int));
	return recvmsg(fd, &hdr, 0);
}
