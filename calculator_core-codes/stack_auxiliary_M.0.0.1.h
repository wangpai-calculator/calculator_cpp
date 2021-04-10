
#pragma once

#include <stack>
#include <vector>
#include <iterator>


using namespace std;


class stack_auxCla//auxCla：auxiliary Class 辅助的类
{
public:

	template <typename DataType>
	static vector<DataType> stack2vector(std::stack<DataType> this_stack)//此形参不能使用const
	{//规定靠近栈顶的序号值小，栈顶的序号值为0

		using namespace std;

		vector<DataType> vector_aux;//aux：auxiliary 辅助

		while (!this_stack.empty())
		{/*这样的方法弹出和插入能保证将stack对象转化为vector对象时，相应元素的序号不会发生改变*/
			auto top_temp = this_stack.top();
			this_stack.pop();
			vector_aux.push_back(top_temp);
		}

		return vector_aux;
	}

	template <typename DataType>
	static const DataType index(const std::stack<DataType>& this_stack, int seqnum)
	{//规定靠近栈顶的序号值小，栈顶的序号值为0
		return (stack2vector(this_stack))[seqnum];//因为stack类无法直接遍历，所以只好先转化为vector
	}

	template <typename DataType>
	static int find(const std::stack<DataType>& this_stack, const DataType& data)
	{//返回值为-1代表未找到.栈顶的序号值为0

		int stack_length = this_stack.size();

		//因为stack类无法直接遍历，所以只好先转化为vector
		std::vector<DataType> vector_aux(stack2vector(this_stack));//aux：auxiliary 辅助

		for (int order = 0; order < stack_length; ++order)
		if (data == vector_aux[order])
			return order;

		return -1;
	}

	template <typename DataType>
	static int empty_stack(std::stack<DataType>& this_stack)//此形参不能使用const
	{//返回值为因清空而删除的栈的元素个数

		int delete_num = 0;

		while (!this_stack.empty())
		{
			this_stack.pop();
			++delete_num;
		}

		return delete_num;
	}

	//将第二个栈other_stack的栈顶拼接到第一个栈this_stack的栈底
	template <typename DataType>
	static std::stack<DataType>& stackCat(std::stack<DataType>& this_stack, const std::stack<DataType>& other_stack)
	{//Cat：catenate 连接。取名灵感自C语言字符串拼接函数strcat

		//拼接需要在第一个stack对象的栈底进行插入，而栈的出栈后再入栈会产生逆序。这些因素导致需要将先stack转化为vector
		vector<DataType> vector_aux_this(stack2vector(this_stack));//aux：auxiliary 辅助 
		vector<DataType> vector_aux_other(stack2vector(other_stack));//aux：auxiliary 辅助

		int this_length = vector_aux_this.size();
		int other_length = vector_aux_other.size();

		empty_stack(this_stack);

		while (other_length)//this_length为0时，最开始的元素刚好已被拷贝，因此可以将this_length直接做为判断条件
		{
			--other_length;//因为数组下标的起点为0，所以这里一开始就要减1
			this_stack.push(vector_aux_other[other_length]);//必须先从第2个vector开始入栈
		}

		while (this_length)//this_length为0时，最开始的元素刚好已被拷贝，因此可以将this_length直接做为判断条件
		{
			--this_length;//因为数组下标的起点为0，所以这里一开始就要减1
			this_stack.push(vector_aux_this[this_length]);//必须先从第2个vector开始入栈
		}

		return this_stack;
	}


};

