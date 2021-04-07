
//这是链栈的实现头文件

#pragma once
#include "list_templet_headnode_M.0.1.h"

template <typename DataType>
class WangpaiStack :virtual public WangpaiList<DataType>
{
protected:
	/*-----------------数据成员---------------------------*/
	static const char* const defaultname;
	/*-----------------数据成员---------------------------*/

public:
	WangpaiStack(const char* stackname = (char*)defaultname, int max_length = INT_MAX)
		:WangpaiList(stackname, max_length){}

	WangpaiStack(const DataType& data, int max_length = INT_MAX)
		:WangpaiList(data, max_length){}

	WangpaiStack(const WangpaiStack& other, int max_length = INT_MAX)
		:WangpaiList(other, max_length){}

	virtual ~WangpaiStack(){}

	virtual DataType& getop()//getop：get top 获取栈顶元素
	{//使用该方法之前，必须保证栈不为空，否则将引发异常
		DataType* pd;
		try
		{
			pd = &searchd(1);
		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{//就算catch后面只有一条语句，也必须使用大括号
			throw "来自函数 virtual DataType& WangpaiStack<DataType>:: getop() 的反馈:\n"
				"   获取栈顶元素失败\n\n";
		}
		return *pd;
	}

	virtual WangpaiStack& push(const DataType& data)
	{
		WangpaiList::forinser(data);
		return *this;
	}

	virtual DataType pop()
	{
		DataType data;
		try
		{
			data = getop();
		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{//就算catch后面只有一条语句，也必须使用大括号
			throw "来自函数 virtual DataType WangpaiStack<DataType>:: pop() 的反馈:\n"
				"   出栈失败\n\n";
		}
		WangpaiList::deldata(1);
		return data;
	}
};


template<typename DataType>
const char* const WangpaiStack<DataType>::defaultname = "这是链栈";
