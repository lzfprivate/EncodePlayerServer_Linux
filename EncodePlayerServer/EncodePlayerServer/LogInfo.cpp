#pragma once

#include <stdarg.h>
#include "LogInfo.h"
#include "CLoggerServer.h"

LogInfo::LogInfo(const char* file, int line, const char* function, pid_t pid, pthread_t tid, int level, const char* fmt, ...) {
	const char spLevel[][8] = { "INFO","DEBUG","WARNING","ERROR","FATAL" };
	char* buf = nullptr;
	m_bStream = false;

	int count = asprintf(&buf, "%s (%d) [%s][%s]<%d-%d>(%s)", file, line, spLevel[level],
		(char*)CLoggerServer::GetCurrTimeBuffer(), pid, tid, function);
	if (count < 0) return;
	else
	{
		m_buffer = buf;
		free(buf);
	}
	va_list vl;
	va_start(vl, fmt);
	count = vasprintf(&buf, fmt, vl);
	if (count > 0)
	{
		m_buffer += buf;
		free(buf);
	}
	m_buffer += "\n";
	va_end(vl);
}
	//
LogInfo::LogInfo(const char* file, int line, const char* function, pid_t pid, pthread_t tid, int level, const char* fmt)
{
	m_bStream = true;
	const char spLevel[][8] = { "INFO","DEBUG","WARNING","ERROR","FATAL" };
	char* buf = nullptr;
	int count = asprintf(&buf, "%s(%d):[%s][%s]<%d-%d>(%s) ",
		file, line, spLevel[level],
		(char*)CLoggerServer::GetCurrTimeBuffer(), pid, tid, function);
	if (count > 0) {
		m_buffer = buf;
		free(buf);
	}
}
	//用于打印二进制信息
LogInfo::LogInfo(const char* file, int line, const char* function, pid_t pid, pthread_t tid, int level, void* data, size_t size) {
	const char spLevel[][8] = { "INFO","DEBUG","WARNING","ERROR","FATAL" };
	char* buf = nullptr;
	m_bStream = false;

	int count = asprintf(&buf, "%s (%d) [%s][%s]<%d-%d>(%s)", file, line, spLevel[level],
		(char*)CLoggerServer::GetCurrTimeBuffer(), pid, tid, function);
	if (count < 0) return;
	else
	{
		m_buffer = buf;
		free(buf);
	}
	CCommonBuffer out;
	size_t i = 0;
	char* Data = (char*)data;
	for (; i < size; i++)
	{
		char buf[16] = "";
		snprintf(buf, sizeof(buf), "%02X ", Data[i] & 0xFF);
		m_buffer += buf;
		if (0 == ((i + 1) % 16)) {
			m_buffer += "\t; ";
			char buf[17] = "";
			memcpy(buf, Data + i - 15, 16);
			for (int j = 0; j < 16; j++)
				if ((buf[j] < 32) && (buf[j] >= 0))buf[j] = '.';
			m_buffer += buf;
			m_buffer += "\n";
		}
	}
	//处理尾巴
	size_t k = i % 16;
	if (k != 0) {
		for (size_t j = 0; j < 16 - k; j++) m_buffer += "   ";
		m_buffer += "\t; ";
		for (size_t j = i - k; j <= i; j++) {
			if ((Data[i] & 0xFF) > 31 && ((Data[j] & 0xFF) < 0x7F)) {
				m_buffer += Data[i];
			}
			else {
				m_buffer += '.';
			}
		}
		m_buffer += "\n";
	}
}

LogInfo::~LogInfo()
{
	if (m_bStream)
	{
		m_buffer += "\n";
		CLoggerServer::Trace(*this);
	}
}
