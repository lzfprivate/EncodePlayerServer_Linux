#pragma once
#include "CommonBuffer.h"
#include <sstream>
enum class LogLevel
{
	LOGINFO = 0,			//普通信息
	LOGDEBUG,				//调试信息
	LOGWARNING,				//警告信息
	LOGERROR,				//错误信息
	LOGFATAL				//系统错误信息
};

class LogInfo
{
public:
	LogInfo(const char* file, int line, const char* function, pid_t pid, pthread_t tid, int level, const char* fmt, ...);
	//
	LogInfo(const char* file, int line, const char* function, pid_t pid, pthread_t tid, int level, const char* fmt);
	//用于打印二进制信息
	LogInfo(const char* file, int line, const char* function, pid_t pid, pthread_t tid, int level, void* data, size_t size);

	~LogInfo();

public:
	operator CCommonBuffer() {
		return m_buffer;
	}

	CCommonBuffer GetTotalBuffer() const 
	{
		return m_buffer;
	}

	template<typename T>
	LogInfo& operator<<(const T& data)
	{
		std::stringstream ss;
		ss << data;
		m_buffer += ss.str();
		return *this;
	}

private:
	CCommonBuffer m_buffer;

	bool m_bStream;				//是否是流式信息

};

