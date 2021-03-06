
//Arithoper：arithmetic operation 算术运算 

#pragma once

#include <cctype>
#include <cmath>
#include <iostream>
#include <stack>

#include "stack_auxiliary_M.0.0.1.h"

using namespace std;//虽然按照规范，在此处使用此代码有隐患，但事实上，这样做很方便

//#include "arithoper_M.0.1.h"

class Arithoper
{
	/*-----------------------------------private权限开始------------------------------------------------------------*/
private:
	stack<double> opnd;//opnd：operand 操作数
	stack<char> opndbuff;//opndbuff：operand buffer 缓存的操作数的每一位的值
	stack<char> optr;//optr：operator 运算符
	stack<char> inte;//inte：integration 当前整个表达式的状态
	stack<char> buttons;//储存得到定义的按键
	stack<char> ndbfbackup;//ndbfbackup：operand buffer backup 缓存区的备份
	//此变量ndbfbackup暂时未起到作用，只是为了保留给以后使用

	/*-----------------------------------private权限结束------------------------------------------------------------*/



	/*-----------------------------------private权限开始------------------------------------------------------------*/
private:
	void inreset()//inreset：input reset 输入重置
	{
		using namespace std;
		cin.clear();
		while ('\n' != cin.get())
			continue;
	}

	ios_base::fmtflags coutset(int n)//将cout小数点后的输出，输出至n位，并且不使用科学计数法
	{
		using namespace std;
		cout.precision(n);
		auto oldset = cout.setf(ios_base::fixed, ios_base::floatfield);
		return oldset;
	}

	void initButton()
	{
		for (char num = '0'; num <= '9'; ++num)//设置数字键
			buttons.push(num);

		buttons.push('=');//设置等于号

		buttons.push('+');//设置加号
		buttons.push('-');//设置减号
		buttons.push('*');//设置乘号
		buttons.push('/');//设置除号

		buttons.push('(');//设置左小括号
		buttons.push(')');//设置右小括号

		buttons.push('.');//设置小数点符号
	}

	bool isDefinedButton(char button)
	{
		if (stack_auxCla::find(buttons, button) != -1)
			return true;
		else
			return false;
	}

	int parematch()//parematch：parentheses match 括号匹配
	{//左右括号相等时，返回1；左括号多于右括号，返回2；左括号小于右括号，返回0
		stack<char> pare(optr);
		stack<char> antipare;//antipare：anti pare pare的反转
		while (!pare.empty())
		{
			auto top_temp = pare.top();
			antipare.push(top_temp);
			pare.pop();
		}
		while (!antipare.empty())
		{
			auto temp = antipare.top();
			antipare.pop();

			switch (temp)
			{
			case '(':
				pare.push(temp);
				break;
			case ')':
				if (!pare.empty() && '(' == pare.top())
					pare.pop();
				else
					return false;
			default:
				break;
			}
		}
		if (pare.empty())//如果最后pare为空，说明左右括号相等
			return true;
		else //如果最后pare不为空，说明左括号数量多于右括号
			return 2;
	}

	char precede(char operleft, char operight) //operleft：operator left 左边的运算符，operight：operator right 右边的运算符
	{//注意：C++本身的语法里面，==的优先级比&&高，&&的优先级比||高
		if (operleft == '(' && operight == ')')
			return '=';
		else if (operight == '.')
			return '<';
		else if (operleft == '(' || operight == '(')
			return '<';
		else if ((operleft == '+' || operleft == '-')
			&& (operight == '*' || operight == '/'))
			return '<';
		else
			return '>';
	}

	void show(const stack<char>& expression)//展示整个表达式，expression自身必须要是以栈保存的逆序（逆序是指后面输入的内容位于栈顶）
	{
		if (true == expression.empty())
			return;
		else
		{
			coutset(10);
			cout << endl;
			for (int order = expression.size() - 1; order >= 0; --order)
				cout << stack_auxCla::index(expression, order);

		}
	}

	// medresult：intermediate result 中间结果
	void show(const stack<char>& expression, const stack<double>& medresult, char delimiter)
	{//这里delimiter有默认参数，默认参数为#，当其为其他符号时，意味着可能将输出最后的定界符前置
		//expression、medresult自身必须要是以栈保存的逆序（逆序是指后面输入的内容位于栈顶）

		if (true == expression.empty())
			return;
		else
		{
			coutset(10);
			cout << endl;
			int reorder_exp = expression.size() - 1;//reorder_exp：reverse order expression expression的倒数序号
			int reorder_med = medresult.size() - 1;//reorder_med：reverse order medresult medresult的倒数序号
			int lowbd = 0;//lowbd：lower bound 下界
			if ('#' != delimiter && delimiter == expression.top())
			{
				cout << delimiter;//expression末尾如果有该定界符，将其前置
				++lowbd;
			}
			for (; reorder_exp >= lowbd; --reorder_exp)
			{//#为转换符
				if ('#' == stack_auxCla::index(expression, reorder_exp))//如果遇到转换符，到medresult里找操作数
				{
					cout << stack_auxCla::index(medresult, reorder_med);
					--reorder_med;
				}
				else
					cout << stack_auxCla::index(expression, reorder_exp);
			}
		}
	}

	bool loadopnd()//将opndbuff内的缓存数字转换成操作数压入操作数栈，loadopnd：load operand 装载操作数
	{//此算法希望尽量不使用链表的方法而使用链栈的方法
		//如果实际装载了操作数，返回true

		if (true == opndbuff.empty())
			return false;

		double sum = 0;
		int point = stack_auxCla::find(opndbuff, '.');

		ndbfbackup = opndbuff;//备用代码，现在没有用
		if (-1 != point)
		{
			stack<char> fraction;
			for (int digit_order = 0; digit_order < point; ++digit_order)
			{//如果有小数部分，先要反转才能弹出
				auto top_temp = opndbuff.top();
				opndbuff.pop();
				fraction.push(top_temp);
			}
			for (int digit_bit = 1; !fraction.empty(); ++digit_bit)
			{
				auto top_temp = fraction.top();
				fraction.pop();
				sum = sum + (top_temp - '0') / pow(10, digit_bit);
			}
			opndbuff.pop();//小数部分已读取完毕，应去掉缓存里的小数点
		}

		for (int digit = 1; !opndbuff.empty(); ++digit)//整数部分可以不用反转直接弹出
		{
			auto top_temp = opndbuff.top();
			opndbuff.pop();
			sum = sum + (top_temp - '0') * pow(10, digit - 1);
		}

		opnd.push(sum);
		return true;
	}

	bool unloadopnd()//将操作数栈里的操作数退回缓冲区，unloadopnd：unload operand 退回操作数
	{//此函数暂时未被使用，只是为了保留给以后使用

		if (true == opnd.empty())
			return false;

		opnd.pop();
		opndbuff = ndbfbackup;
		return true;
	}

	double calculate(double opndleft, char optr_, double opndright)
	{//计算两数运算结果，如果optr_是下面未给出的运算符，那程序运行以后将报弹窗错误

		switch (optr_)
		{
		case '+':
			return opndleft + opndright;
		case '-':
			return opndleft - opndright;
		case '*':
			return opndleft * opndright;
		case '/':
			return opndleft / opndright;
		}
	}

	/*此函数与evalexpre()函数中显示表达式的过程还是有区别的，所以不能将它们之中显示的部分提取出来制成一个单独的函数为它们调用。
	它们的区别在于：
	1.刚开始用户的输入一定全是字符，而后续计算时会产生浮点数。因此各自使用的显示函数将不同
	2.刚开始的显示(evalexpre)涉及计算过程中，对中间结果异常（如0除的问题）的处理
	3.显示过程(showcalpro)需要涉及栈的合并、以及美观上的处理*/
	void showcalpro()//showcalpro：show the calculation process 展示计算过程
	{//此函数默认inte里面没有语法错误，而且inte没有被清空
		//此函数会首先清空opnd、opndbuff、optr的内容

		if (inte.empty())
		{
			cout << endl << "表达式不存在，无法展示细节" << endl;
			return;
		}

		bool flag = false;//如果flag为true，说明本函数至少执行了一次计算

		auto inte_temp = inte;
		if ('=' == inte_temp.top())
			inte_temp.pop();
		show(inte_temp);

		auto intedup = inte;//intedup：inte duplicate inte的备份,同时充当本函数的integration
		auto antinte = inte;//antinte：anti inte inte的反转
		stack_auxCla::empty_stack(antinte);
		while (!intedup.empty())
		{
			auto top_temp = intedup.top();
			intedup.pop();
			antinte.push(top_temp);
		}
		stack_auxCla::empty_stack(opnd);
		stack_auxCla::empty_stack(opndbuff);
		stack_auxCla::empty_stack(optr);

		auto ch_being_read = antinte.top();//ch_being_read：characters being read 正在读取的字符
		antinte.pop();

		while (!antinte.empty() || intedup.empty() || '=' != intedup.top())
		{
			if ('=' == ch_being_read && optr.empty())//如果循环对=前面的操作已处理完毕
			{
				if (loadopnd())
					intedup.push('#');//'#'为转换符，以后如在intedup遇到此符，说明此处是操作数，需要去别处寻找

				optr.push(ch_being_read);
				intedup.push(ch_being_read);
				continue;/*这是为了跳过下面的input输入，因为能执行这个代码块，
						 说明表达式已到尽头，不需要新的输入而直接将“=”入栈能马上跳出该循环*/
			}
			else if (isdigit(ch_being_read) || '.' == ch_being_read)//如果是数字或小数点的话，入操作数临时栈
			{
				opndbuff.push(ch_being_read);
			}
			else
			{
				if (loadopnd())
					intedup.push('#');//'#'为转换符，以后如在intedup遇到此符，说明此处是操作数，需要去别处寻找

				if (optr.empty())
				{
					optr.push(ch_being_read);
					intedup.push(ch_being_read);
					goto input;
				}
				switch (precede(optr.top(), ch_being_read))
				{
				case '<':
					optr.push(ch_being_read);
					intedup.push(ch_being_read);
					break;

				case '>':
				{//case中如有变量初始化一定要加{},不然会报“初始化操作由case标签跳过”的错误

							auto optr_ = optr.top();
							optr.pop();
							auto opndright = opnd.top();
							opnd.pop();
							auto opndleft = opnd.top();
							opnd.pop();

							for (int i = 1; i <= 3; ++i)
								intedup.pop();//弹出两个操作数、一个运算符，共三个字符
							double medresult = calculate(opndleft, optr_, opndright);
							// medresult：intermediate result 中间结果
							opnd.push(medresult);
							intedup.push('#');//'#'为转换符，遇到此符，说明此处是操作数，需要去别处寻找

							/*下面将显示表达式本步运算的相关内容。这需要同时显示前面已经计算的操作intedup、
							当前的符号ch_being_read、以及尚未读取的表达式antinte*/

							//因为antinte其实是正序的，而这里show的传参必须是栈的逆序，所以这里要进行复杂的倒序
							auto poleft = antinte;//poleft：the leftover in positive order 剩下的正序元素 
							poleft.push(ch_being_read);
							auto reverleft = poleft;//reverleft：the leftover in reverse order 剩下的逆序元素
							stack_auxCla::empty_stack(reverleft);
							while (!poleft.empty())
							{
								auto top_temp = poleft.top();
								reverleft.push(top_temp);
								poleft.pop();

							}

							auto fulexp = reverleft;//fulexp：full expression 完整表达式
							/*因为show的传参需要栈逆序，而intedup本身就是逆序，所以这里是直接将intedup接在reverleft的后面*/
							stack_auxCla::stackCat(fulexp, intedup);

							cout << endl;
							show(fulexp, opnd, '=');
							flag = true;//这是为了告诉程序，本函数至少执行了一次计算	

							continue;/*这是为了跳过下面的input输入，因为能执行这个代码块，
									 说明ch_being_read的优先级比较低，因此尚未入栈（未经过处理），所以不需要新的输入*/
				}
					break;

				case '='://如果两个括号之间没有其他运算符，就只有一个操作数
				{//case中如有变量初始化一定要加{},不然会报“初始化操作由case标签跳过”的错误
							 optr.pop();//弹出运算符栈的左括号


							 auto opnd_temp = intedup.top();

							 /*现在弹出的不是左括号，因为intedup中的左括号离栈顶还有一个操作数。
							 而两个括号之间的操作数不是我们想要弹出的，因此要先保存该操作数*/
							 intedup.pop();

							 intedup.pop();//弹出左括号
							 intedup.push(opnd_temp);//将前面被迫先暂时弹出的操作数再加入栈中
				}
					break;

				}//switch的右括号
			}//else的右括号


		input:
			ch_being_read = antinte.top();
			antinte.pop();
		}//while的右括号

		if (!flag)//flag不为真，说明很可能用户输入的是一个不需要计算的表达式
		{
			inte_temp.push('=');//inte_temp是本函数开始出保存inte的一个对象，它的“=”之前被弹出，因此这里把它弹回去
			show(inte_temp, opnd, '=');
		}

	}
	/*-----------------------------------private权限结束------------------------------------------------------------*/


	/*-----------------------------------public权限开始------------------------------------------------------------*/
public:

	Arithoper()
	{
		initButton();//初始化按键
	}

	void evalexpre()//evalexpre：evaluate expression 对表达式求值
	{
		cout << "请输入一个表达式（你可以选择一个一个地输入，也可以选择一次性输完）：\n"
			<< "(请注意除号是“/”，而不是“\\”)\n";

		using namespace std;
		show(inte);
		char characters_entered;
		cin >> characters_entered;
		while (optr.empty() || '=' != optr.top())//以=为结束标志
		{
			//先尽可能处理所有能单独判断的语法错误
			if (!cin)
			{
				cout << endl << "你进行了意外的输入，已为你重置" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (!isDefinedButton(characters_entered))
			{
				cout << endl << "你输入了未定义按键，已为你清除" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('.' == characters_entered&& -1 != stack_auxCla::find(opndbuff, '.'))//一个操作数里，输入了两个小数点
			{
				cout << endl << "你不能在一个数里输入两个" << "“" << characters_entered << "”"
					<< "，已为你自动删除" << "“" << characters_entered << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (!opndbuff.empty() && '0' == opndbuff.top() && 1 == opndbuff.size()
				&& '.' != characters_entered)//如果输入的数字不是小于1的小数，但是其最高位又为0
			{
				cout << endl << "你不能一开始就在数中输入" << "“" << opndbuff.top() << "”"
					<< "，已为你自动删除" << "“" << opndbuff.top() << "”" << "：" << endl;
				opndbuff.pop();//删除一开始输入的0
				inte.pop();
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == characters_entered || '-' == characters_entered
				|| '*' == characters_entered || '/' == characters_entered
				|| '.' == characters_entered) &&
				(inte.empty() || (!isdigit(inte.top()) && inte.top() != ')')))
				//如果前面没有第一个操作数，但是后面输入了一个非数字符，报错
			{
				cout << endl << "注意：在输入" << "“" << characters_entered << "”" << "前，你必须先输入一个合法的数。"
					<< "已为你自动删除“" << characters_entered << "”：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == characters_entered || '-' == characters_entered
				|| '*' == characters_entered || '/' == characters_entered
				|| '=' == characters_entered || '.' == characters_entered)
				&& !inte.empty() && ('+' == inte.top() || '-' == inte.top() || '*' == inte.top()
				|| '/' == inte.top() || '(' == inte.top() || '.' == inte.top()))//连续输入两个非数字符，报错
			{
				cout << endl << "你不能在" << "“" << inte.top() << "”" << "的后面输入"
					<< "“" << characters_entered << "”" << "，已为你自动删除" << "“" << characters_entered << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('(' == characters_entered && !inte.empty() && isdigit(inte.top())
				&& '.' == characters_entered)//如果左括号前面有数字、小数点，报错
			{
				cout << endl << "左括号前面不能有数字，已为你自动删除" << "“" << characters_entered << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (')' == characters_entered)
			{
				if (!inte.empty() && (('+' == inte.top() || '-' == inte.top()
					|| '*' == inte.top() || '/' == inte.top())))//如果右括号前面有四则运算符，报错
				{
					cout << endl << "右括号前面不能有四则运算符，已为你自动删除" << "“" << characters_entered << "”" << "：" << endl;
					inreset();
					show(inte);
					goto input;
				}
				if (!inte.empty() && '(' == inte.top())
				{
					cout << endl << "这一对括号里什么也没有，已为你自动删除这一对括号：" << endl;
					inreset();
					optr.pop();
					inte.pop();
					show(inte);
					goto input;
				}

				optr.push(characters_entered);//等下就要检查括号匹配的问题，而括号匹配必须先入栈才能检查
				int matchresult = parematch();//括号匹配检查
				optr.pop();//刚刚只是为了检查括号匹配而入栈，这里来消除这种检查的副作用
				if (false == matchresult)//如果右括号不匹配，报错
				{
					cout << endl << "右括号不匹配，已为你自动删除" << "“" << characters_entered << "”" << "：" << endl;
					inreset();
					show(inte);
					goto input;
				}
			}
			if ('=' == characters_entered)
			if (2 == parematch())//最后准备结束输入时，如果发现左括号多于右括号，报错
			{
				cout << endl << "左括号不能多于右括号，已为你自动删除" << "“" << characters_entered << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}

			//现在的正常情况下的处理
			//注意：此区域if else判断语句后面紧挨着的就是输入语句，所以这里的代码块中可能可以省略“goto input”部分
			if ('=' == characters_entered && optr.empty())//如果循环对=前面的操作已处理完毕，那么这里将做收尾工作用以结束循环
			{
				loadopnd();
				optr.push(characters_entered);//此处让“=”入栈的目的是为了下次能结束循环
				inte.push(characters_entered);
				show(inte);
				continue;//跳过最后的输入部分。因为现在不需要再输入
			}
			else if (isdigit(characters_entered) || '.' == characters_entered)//如果是数字或小数点的话，入操作数临时栈
			{
				opndbuff.push(characters_entered);
				inte.push(characters_entered);
				show(inte);
			}
			else
			{
				loadopnd();
				if (optr.empty())//如果运算符栈里什么也没有，该运算符就直接入栈
				{
					optr.push(characters_entered);
					inte.push(characters_entered);
					show(inte);
					goto input;//因为外层的else块还有剩余的switch块，所以这里不能省略此语句
				}
				switch (precede(optr.top(), characters_entered))
				{/*此switch语句结束后，就直接是else的结束部分，而后者结束后就到了输入部分，
				 所以这里“break”可能可以替代“goto input”*/

				case '<':
					optr.push(characters_entered);
					inte.push(characters_entered);
					show(inte);
					break;

				case '>':
					if ('(' != characters_entered && '/' == optr.top() && !opnd.empty() && 0 == opnd.top())//如果把0作为除数
					{
						if (!inte.empty() && '0' == inte.top())
						{
							cout << endl << "抱歉，0不能作除数，已为你自动删除：" << endl;
							inreset();
							opnd.pop();
							inte.pop();
							show(inte);
							break;
						}
						else
						{
							cout << endl << "中间有的算式让除数为0，本次计算失败，正在为你定位到新输入：" << endl;
							inreset();
							stack_auxCla::empty_stack(opnd);
							stack_auxCla::empty_stack(opndbuff);
							stack_auxCla::empty_stack(optr);
							stack_auxCla::empty_stack(inte);
							break;
						}
					}
					{//case中如有变量初始化一定要加{},不然会报初始化操作由case标签跳过

						auto optr_ = optr.top();
						optr.pop();
						auto opndright = opnd.top();
						opnd.pop();
						auto opndleft = opnd.top();
						opnd.pop();
						opnd.push(calculate(opndleft, optr_, opndright));
						continue;//跳过最后的输入部分。因为这里通过计算产生了新的操作数，所以不需要输入
					}
					break;

				case '=':
					optr.pop();
					inte.push(characters_entered);
					show(inte);
					break;

				}//switch的右括号
			}//else的右括号

		input:	cin >> characters_entered;
		}//while的右括号

		if (opnd.empty())
		{
			cout << endl << "此表达式为空，计算失败" << endl;
			inreset();

			stack_auxCla::empty_stack(opnd);
			stack_auxCla::empty_stack(opndbuff);
			stack_auxCla::empty_stack(optr);
			stack_auxCla::empty_stack(inte);
		}
		else
		{
			cout << "\n\n输入结束，下面是计算结果：\n";
			show(inte);//输出表达式（包括最后的“=”）
			cout << opnd.top();//在“=”后输出计算结果

			cout << "\n\n下面是计算过程：\n";
			showcalpro();
			cout << endl << endl;

			inreset();

			stack_auxCla::empty_stack(opnd);
			stack_auxCla::empty_stack(opndbuff);
			stack_auxCla::empty_stack(optr);
			stack_auxCla::empty_stack(inte);
		}
	}//evalexpre函数的右括号
	/*-----------------------------------public权限结束------------------------------------------------------------*/


};//Arithoper class的右括号、分号
