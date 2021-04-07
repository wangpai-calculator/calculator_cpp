
//������ջ��ʵ��ͷ�ļ�

#pragma once
#include "list_templet_headnode_M.0.1.h"

template <typename DataType>
class WangpaiStack :virtual public WangpaiList<DataType>
{
protected:
	/*-----------------���ݳ�Ա---------------------------*/
	static const char* const defaultname;
	/*-----------------���ݳ�Ա---------------------------*/

public:
	WangpaiStack(const char* stackname = (char*)defaultname, int max_length = INT_MAX)
		:WangpaiList(stackname, max_length){}

	WangpaiStack(const DataType& data, int max_length = INT_MAX)
		:WangpaiList(data, max_length){}

	WangpaiStack(const WangpaiStack& other, int max_length = INT_MAX)
		:WangpaiList(other, max_length){}

	virtual ~WangpaiStack(){}

	virtual DataType& getop()//getop��get top ��ȡջ��Ԫ��
	{//ʹ�ø÷���֮ǰ�����뱣֤ջ��Ϊ�գ����������쳣
		DataType* pd;
		try
		{
			pd = &searchd(1);
		}
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{//����catch����ֻ��һ����䣬Ҳ����ʹ�ô�����
			throw "���Ժ��� virtual DataType& WangpaiStack<DataType>:: getop() �ķ���:\n"
				"   ��ȡջ��Ԫ��ʧ��\n\n";
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
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{//����catch����ֻ��һ����䣬Ҳ����ʹ�ô�����
			throw "���Ժ��� virtual DataType WangpaiStack<DataType>:: pop() �ķ���:\n"
				"   ��ջʧ��\n\n";
		}
		WangpaiList::deldata(1);
		return data;
	}
};


template<typename DataType>
const char* const WangpaiStack<DataType>::defaultname = "������ջ";
