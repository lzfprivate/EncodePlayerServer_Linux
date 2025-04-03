#pragma once
#include "FunctionBase.h"
#include <functional>
#include <unistd.h>


template<typename _FUNCTION_,typename... _ARGS_>
class CFuntionProcess :
    public CFunctionBase
{
public:
    CFuntionProcess(_FUNCTION_ function, _ARGS_... args)
        :m_binder(std::forward<_FUNCTION_>(function),std::forward<_ARGS_>(args)...)
    {

    }
    virtual ~CFuntionProcess()
    {

    }
    
    virtual int operator()() {
        return m_binder();
    }


private:
    typename std::_Bindres_helper<int, _FUNCTION_, _ARGS_...>::type m_binder;

};

