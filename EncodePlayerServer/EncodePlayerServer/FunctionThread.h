#pragma once
#include "FunctionBase.h"
#include <functional>

template <typename _FUNCTION_,typename... _ARGS_>
class CFunctionThread :
    public CFunctionBase
{
public:
	CFunctionThread(_FUNCTION_ function, _ARGS_... args)
		:m_binder(std::forward<_FUNCTION_>(function), std::forward<_ARGS_>(args)...)
	{

	}
	virtual ~CFunctionThread()
	{

	}

	virtual int operator()() override
	{
		return m_binder();
	}

private:
	typename std::_Bindres_helper<int, _FUNCTION_, _ARGS_...>::type m_binder;
	
};

