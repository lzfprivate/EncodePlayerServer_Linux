#pragma once

class CFunctionBase
{
public:
	virtual ~CFunctionBase()
	{

	}

	virtual int operator()() = 0;
};

