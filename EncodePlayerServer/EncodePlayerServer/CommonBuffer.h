#pragma once
#include <string>
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

    operator char* () { return (char*)c_str(); }
    operator char* () const { return (char*)c_str(); }
    operator const char* () const { return c_str(); }

};

