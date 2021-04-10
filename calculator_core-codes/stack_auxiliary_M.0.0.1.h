
#pragma once

#include <stack>
#include <vector>
#include <iterator>


using namespace std;


class stack_auxCla//auxCla��auxiliary Class ��������
{
public:

	template <typename DataType>
	static vector<DataType> stack2vector(std::stack<DataType> this_stack)//���ββ���ʹ��const
	{//�涨����ջ�������ֵС��ջ�������ֵΪ0

		using namespace std;

		vector<DataType> vector_aux;//aux��auxiliary ����

		while (!this_stack.empty())
		{/*�����ķ��������Ͳ����ܱ�֤��stack����ת��Ϊvector����ʱ����ӦԪ�ص���Ų��ᷢ���ı�*/
			auto top_temp = this_stack.top();
			this_stack.pop();
			vector_aux.push_back(top_temp);
		}

		return vector_aux;
	}

	template <typename DataType>
	static const DataType index(const std::stack<DataType>& this_stack, int seqnum)
	{//�涨����ջ�������ֵС��ջ�������ֵΪ0
		return (stack2vector(this_stack))[seqnum];//��Ϊstack���޷�ֱ�ӱ���������ֻ����ת��Ϊvector
	}

	template <typename DataType>
	static int find(const std::stack<DataType>& this_stack, const DataType& data)
	{//����ֵΪ-1����δ�ҵ�.ջ�������ֵΪ0

		int stack_length = this_stack.size();

		//��Ϊstack���޷�ֱ�ӱ���������ֻ����ת��Ϊvector
		std::vector<DataType> vector_aux(stack2vector(this_stack));//aux��auxiliary ����

		for (int order = 0; order < stack_length; ++order)
		if (data == vector_aux[order])
			return order;

		return -1;
	}

	template <typename DataType>
	static int empty_stack(std::stack<DataType>& this_stack)//���ββ���ʹ��const
	{//����ֵΪ����ն�ɾ����ջ��Ԫ�ظ���

		int delete_num = 0;

		while (!this_stack.empty())
		{
			this_stack.pop();
			++delete_num;
		}

		return delete_num;
	}

	//���ڶ���ջother_stack��ջ��ƴ�ӵ���һ��ջthis_stack��ջ��
	template <typename DataType>
	static std::stack<DataType>& stackCat(std::stack<DataType>& this_stack, const std::stack<DataType>& other_stack)
	{//Cat��catenate ���ӡ�ȡ�������C�����ַ���ƴ�Ӻ���strcat

		//ƴ����Ҫ�ڵ�һ��stack�����ջ�׽��в��룬��ջ�ĳ�ջ������ջ�����������Щ���ص�����Ҫ����stackת��Ϊvector
		vector<DataType> vector_aux_this(stack2vector(this_stack));//aux��auxiliary ���� 
		vector<DataType> vector_aux_other(stack2vector(other_stack));//aux��auxiliary ����

		int this_length = vector_aux_this.size();
		int other_length = vector_aux_other.size();

		empty_stack(this_stack);

		while (other_length)//this_lengthΪ0ʱ���ʼ��Ԫ�ظպ��ѱ���������˿��Խ�this_lengthֱ����Ϊ�ж�����
		{
			--other_length;//��Ϊ�����±�����Ϊ0����������һ��ʼ��Ҫ��1
			this_stack.push(vector_aux_other[other_length]);//�����ȴӵ�2��vector��ʼ��ջ
		}

		while (this_length)//this_lengthΪ0ʱ���ʼ��Ԫ�ظպ��ѱ���������˿��Խ�this_lengthֱ����Ϊ�ж�����
		{
			--this_length;//��Ϊ�����±�����Ϊ0����������һ��ʼ��Ҫ��1
			this_stack.push(vector_aux_this[this_length]);//�����ȴӵ�2��vector��ʼ��ջ
		}

		return this_stack;
	}


};

