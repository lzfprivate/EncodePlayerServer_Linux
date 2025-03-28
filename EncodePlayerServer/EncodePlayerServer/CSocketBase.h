#pragma once
#include <sys/socket.h>
#include <unistd.h>
class CSocketBase
{
public:
	virtual ~CSocketBase()
	{
		Close();
	}

	virtual int Initialize() = 0;
	virtual int Link() = 0;
	virtual int Send() = 0;
	virtual int Recv() = 0;
	void Close() {
		if (m_socket != -1)
		{
			int fd = m_socket;
			m_socket = -1;
			close(fd);
		}
	}

private:
	int m_socket;

};

