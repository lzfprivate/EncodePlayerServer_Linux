#pragma once
#include <string>
#include <string.h>
class CCommonBuffer :
    public std::string
{
public:
    CCommonBuffer() : std::string()
    {

    }
    CCommonBuffer(unsigned int size) : std::string()
    {
        resize(size);
    }

    CCommonBuffer(const char* buffer) : std::string(buffer)
    {
        resize(strlen(buffer));
        memcpy((void*)c_str(), buffer, strlen(buffer));
    }

    operator char* () { return (char*)c_str(); }
    operator char* () const { return (char*)c_str(); }
    operator const char* () const { return c_str(); }

    CCommonBuffer& operator=(const char* buffer) 
    {
        resize(strlen(buffer));
        memcpy((void*)c_str(), buffer, strlen(buffer));
        return  *this;
    }
    CCommonBuffer& operator=(const std::string& str)
    {
        resize(str.size());
        memcpy((void*)c_str(), str.c_str(), str.size());
        return  *this;
    }

};

