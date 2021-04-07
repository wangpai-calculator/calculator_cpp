
//


#include "arithoper.h"

namespace arithoper
{
	using namespace std;
	WangpaiStack<double> opnd;//opnd：operand 操作数
	WangpaiStack<char> opndbuff;//opndbuff：operand buffer 缓存的操作数的每一位的值
	WangpaiStack<char> optr;//optr：operator 运算符
	WangpaiStack<char> inte;//inte：integration 当前整个表达式的状态
	WangpaiStack<char> ndbfbackup;//ndbfbackup：operand buffer backup 缓存区的备份
	//此变量ndbfbackup暂时未起到作用，只是为了保留给以后使用

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

	int parematch()//parematch：parentheses match 括号匹配
	{//左右括号相等时，返回1；左括号多于右括号，返回2；左括号小于右括号，返回0
		WangpaiStack<char> pare(optr);
		WangpaiStack<char> antipare;//antipare：anti pare pare的反转
		while (!pare.isemp())
			antipare.push(pare.pop());
		while (!antipare.isemp())
		{
			auto temp = antipare.pop();
			switch (temp)
			{
			case '(':
				pare.push(temp);
				break;
			case ')':
				if (!pare.isemp() && '(' == pare.getop())
					pare.pop();
				else
					return false;
			default:
				break;
			}
		}
		if (pare.isemp())//如果最后pare为空，说明左右括号相等
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

	void show(WangpaiStack<char>& expression)//展示整个表达式，expression自身必须要是以栈保存的逆序
	{
		if (true == expression.isemp())
			return;
		else
		{
			coutset(10);
			cout << endl;
			for (int order = expression.count(); order >= 1; --order)
				cout << expression[order];
		}
	}

	void show2(WangpaiStack<char>& expression, WangpaiStack<double>& medresult, char delimiter)// medresult：intermediate result 中间结果
	{//这里delimiter有默认参数，默认参数为#，当其为其他符号时，意味着可能将输出最后的定界符前置
	//expression、medresult自身必须要是以栈保存的逆序
		if (true == expression.isemp())
			return;
		else
		{
			coutset(10);
			cout << endl;
			int order1 = expression.count();
			int order2 = medresult.count();
			int lowbo = 1;//lowbo：lower bound 下界
			if ('#' != delimiter && delimiter == expression[1])
			{
				cout << delimiter;//expression末尾如果有该定界符，将其前置
				++lowbo;
			}
			for (; order1 >= lowbo; --order1)
			{//#为转换符
				if ('#' == expression[order1])//如果遇到转换符，到medresult里找操作数
				{
					cout << medresult[order2];
					--order2;
				}
				else
				cout << expression[order1];
			}
		}
	}

	bool loadopnd()//将opndbuff内的缓存数字转换成操作数压入操作数栈，loadopnd：load operand 装载操作数
	{//此算法希望尽量不使用链表的方法而使用链栈的方法
	 //如果实际装载了操作数，返回true
		if (true == opndbuff.isemp())
			return false;

		double sum = 0;
		int length = opndbuff.count();
		int point = opndbuff.search('.');
		ndbfbackup = opndbuff;//备用代码，现在没有用
		if (-1 != point)
		{
			WangpaiStack<char> fraction;
			for (int digit = 1; digit < point; ++digit)
				fraction.push(opndbuff.pop());//如果有小数部分，先要反转才能弹出
			for (int digit = 1; !fraction.isemp(); ++digit)
				sum = sum + (fraction.pop() - '0') / pow(10, digit);
			opndbuff.pop();//小数部分已读取完毕，应去掉缓存里的小数点
		}

		for (int digit = 1; !opndbuff.isemp(); ++digit)//整数部分可以不用反转直接弹出
			sum = sum + (opndbuff.pop() - '0') * pow(10, digit-1);

		opnd.push(sum);
		return true;
	}

	bool unloadopnd()//将操作数栈里的操作数退回缓冲区，unloadopnd：unload operand 退回操作数
	{//此函数暂时未被使用，只是为了保留给以后使用
		if (true == opnd.isemp())
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
		case '.'://此处opndright必须要是非负数
			while (opndright >= 1)
				opndright /= 10;
			return opndleft + opndright;
		}
	}

	void showcalpro()//showcalpro：show the calculation process 展示计算过程
	{//此函数默认inte里面没有语法错误，而且inte没有被清空
     //此函数会首先清空opnd、opndbuff、optr的内容
		if (inte.isemp())
		{
			cout << endl << "表达式不存在，无法展示细节" << endl;
			return;
		}

		bool flag = false;//如果flag为true，说明本函数至少执行了一次计算

		auto inte_temp = inte;
		if ('=' == inte_temp.getop())
			inte_temp.pop();
		show(inte_temp);

		auto intedup = inte;//intedup：inte duplicate inte的备份,同时充当本函数的integration
		auto antinte = inte;//antinte：anti inte inte的反转
		antinte.emplist();
		while (!intedup.isemp())
			antinte.push(intedup.pop());
		opnd.emplist();
		opndbuff.emplist();
		optr.emplist();
		char ch = antinte.pop();
		
		while (!antinte.isemp() || intedup.isemp() || '='!=intedup.getop())
		{
			if ('=' == ch && optr.isemp())//如果循环对=前面的操作已处理完毕
			{
				if (loadopnd())
					intedup.push('#');//'#'为转换符，以后如在intedup遇到此符，说明此处是操作数，需要去别处寻找

				optr.push(ch);
				intedup.push(ch);
				continue;/*这是为了跳过下面的input输入，因为能执行这个代码块，
						 说明表达式已到尽头，不需要新的输入而直接将“=”入栈能马上跳出该循环*/
			}
			else if (isdigit(ch))//如果是数字的话，入操作数临时栈
			{
				opndbuff.push(ch);
			}
			else
			{
				if (loadopnd())
					intedup.push('#');//'#'为转换符，以后如在intedup遇到此符，说明此处是操作数，需要去别处寻找

				if (optr.isemp())
				{
					optr.push(ch);
					intedup.push(ch);
					goto input;
				}
				switch (precede(optr.getop(), ch))
				{
				case '<':
					optr.push(ch);
					intedup.push(ch);
					break;
				case '>':
					{//case中如有变量初始化一定要加{},不然会报初始化操作由case标签跳过
						bool pointflag = false;//如果下面是小数点导致的运算，此标志将置true
						char optr_ = optr.pop();
						if ('.' == optr_)
							pointflag = true;
						double opndright = opnd.pop();
						double opndleft = opnd.pop();
						for (int i = 1; i <= 3; ++i)
							intedup.pop();//弹出两个操作数、一个运算符，共三个字符
						double medresult = calculate(opndleft, optr_, opndright);
						// medresult：intermediate result 中间结果
						opnd.push(medresult);
						intedup.push('#');//'#'为转换符，遇到此符，说明此处是操作数，需要去别处寻找
								
						if (!pointflag)//如果是小数点导致的运算，不计入本程序的一次运算、将跳过if里面的内容
						{   //下面将显示表达式本步运算的相关内容。这需要同时显示前面已经计算的操作intedup、当前的符号ch、以及尚未读取的表达式antinte
							
							//因为antinte其实是正序的，而这里show2的传参必须是栈的逆序，所以这里要进行复杂的倒序
							auto poleft = antinte;//poleft：the leftover in positive order 剩下的正序元素 
							poleft.push(ch);
							auto reverleft = poleft;//reverleft：the leftover in reverse order 剩下的逆序元素
							reverleft.emplist();
							while (!poleft.isemp())
								reverleft.push(poleft.pop());
							auto fulexp = reverleft;//fulexp：full expression 完整表达式
							fulexp += intedup;//因为show2的传参需要栈逆序，而intedup本身就是逆序，所以这里是直接将intedup接在reverleft的后面

							cout << endl;
							show2(fulexp, opnd, '=');
							flag = true;//这是为了告诉程序，本函数至少执行了一次计算
						}
						continue;/*这是为了跳过下面的input输入，因为能执行这个代码块，
								 说明ch的优先级比较低，因此尚未入栈（未经过处理），所以不需要新的输入*/
					}
				case '=':
					{//如果两个括号之间没有其他运算符，就只有一个操作数
						optr.pop();//弹出运算符栈的左括号
						auto opnd_temp = intedup.pop();/*现在弹出的不是左括号，因为intedup中的左括号离栈顶还有一个操作数。
													   而两个括号之间的操作数不是我们想要弹出的，因此要先保存该操作数*/
						intedup.pop();//弹出左括号
						intedup.push(opnd_temp);//将前面被迫先暂时弹出的操作数再加入栈中
					}	
					break;
				}//switch的右括号
			}//else的右括号

		input:	ch = antinte.pop();
		}//while的右括号

		if (!flag)//flag不为真，说明很可能用户输入的是一个不需要计算的表达式
		{
			inte_temp.push('=');//inte_temp是本函数开始出保存inte的一个对象，它的“=”之前被弹出，因此这里把它弹回去
			show2(inte_temp, opnd, '=');
		}

	}

	void evalexpre()//evalexpre：evaluate expression 对表达式求值
	{
		cout << "请输入一个表达式（你可以选择一个一个地输入，也可以选择一次性输完）：\n"
			<< "(请注意除号是“/”，而不是“\\”)\n";

		using namespace std;
		show(inte);
		char ch;
		cin >> ch;
		while (optr.isemp() || '=' != optr.getop())//以=为结束标志
		{
			//先尽可能处理所有能单独判断的语法错误
			if (!cin)
			{
				cout << endl << "你进行了意外的输入，已为你重置" << endl;
				inreset();
				show(inte);
				goto input;
			}	
			if (('.' == ch) && !optr.isemp() && ('.' == optr.getop()))//一个操作数里，输入了两个小数点
			{
				cout << endl << "你不能在一个数里输入两个" << "“" << ch << "”"
					<< "，已为你自动删除" << "“" << ch << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == ch || '-' == ch || '*' == ch || '/' == ch || '.' == ch) &&
				(inte.isemp() || (!isdigit(inte.getop()) && inte.getop() != ')')))
				//如果前面没有第一个操作数，但是后面输入了一个非数字符，报错
			{
				cout << endl << "注意：在输入" << "“" << ch << "”" << "前，你必须先输入一个合法的数。"
					<< "已为你自动删除“" << ch << "”：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == ch || '-' == ch || '*' == ch || '/' == ch || '=' == ch || '.' == ch)
				&& !inte.isemp() && ('+' == inte.getop() || '-' == inte.getop() || '*' == inte.getop()
				|| '/' == inte.getop() || '(' == inte.getop() || '.' == inte.getop()))//连续输入两个非数字符，报错
			{
				cout << endl << "你不能在" << "“" << inte.getop() << "”" << "的后面输入"
					<< "“" << ch << "”" << "，已为你自动删除" << "“" << ch << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('(' == ch && !inte.isemp() && isdigit(inte.getop()) && ('.' == ch))//如果左括号前面有数字、小数点，报错
			{
				cout << endl << "左括号前面不能有数字，已为你自动删除" << "“" << ch << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (')' == ch)
			{
				if (!inte.isemp() && (('+' == inte.getop() || '-' == inte.getop()
					|| '*' == inte.getop() || '/' == inte.getop())))//如果右括号前面有四则运算符，报错
				{
					cout << endl << "右括号前面不能有四则运算符，已为你自动删除" << "“" << ch << "”" << "：" << endl;
					inreset();
					show(inte);
					goto input;
				}
				if (!inte.isemp() && '(' == inte.getop())
				{
					cout << endl << "这一对括号里什么也没有，已为你自动删除这一对括号：" << endl;
					inreset();
					optr.pop();
					inte.pop();
					show(inte);
					goto input;
				}

				optr.push(ch);//等下就要检查括号匹配的问题，而括号匹配必须先入栈才能检查
				int matchresult = parematch();//括号匹配检查
				optr.pop();//刚刚只是为了检查括号匹配而入栈，这里来消除这种检查的副作用
				if (false == matchresult)//如果右括号不匹配，报错
				{
					cout << endl << "右括号不匹配，已为你自动删除" << "“" << ch << "”" << "：" << endl;
					inreset();
					show(inte);
					goto input;
				}
			}
			if ('=' == ch)
			if (2 == parematch())//最后准备结束输入时，如果发现左括号多于右括号，报错
			{
				cout << endl << "左括号不能多于右括号，已为你自动删除" << "“" << ch << "”" << "：" << endl;
				inreset();
				show(inte);
				goto input;
			}
	
			//现在的正常情况下的处理
			//注意：此区域if else判断语句后面紧挨着的就是输入语句，所以这里的代码块中可能可以省略“goto input”部分
			if ('=' == ch && optr.isemp())//如果循环对=前面的操作已处理完毕，那么这里将做收尾工作用以结束循环
			{
				loadopnd();
				optr.push(ch);//此处让“=”入栈的目的是为了下次能结束循环
				inte.push(ch);
				show(inte);
				continue;//跳过最后的输入部分。因为现在不需要再输入
			}
			else if (isdigit(ch))//如果是数字的话，入操作数临时栈
			{
				opndbuff.push(ch);
				inte.push(ch);
				show(inte);
			}
			else
			{
				loadopnd();
				if (optr.isemp())//如果运算符栈里什么也没有，该运算符就直接入栈
				{
					optr.push(ch);
					inte.push(ch);
					show(inte);
					goto input;//因为外层的else块还有剩余的switch块，所以这里不能省略此语句
				}
				switch (precede(optr.getop(), ch))
				{/*此switch语句结束后，就直接是else的结束部分，而后者结束后就到了输入部分，所以这里“break”可能可以替代“goto input”*/

				case '<':
					optr.push(ch);
					inte.push(ch);
					show(inte);
					break;
				case '>':
					if ('(' != ch && '/' == optr.getop() && !opnd.isemp() && 0 == opnd.getop())//如果把0作为除数
					{
						if (!inte.isemp() && '0' == inte.getop())
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
							opnd.emplist();
							opndbuff.emplist(); 
							optr.emplist(); 
							inte.emplist();
							break;
						}
					}
					{//case中如有变量初始化一定要加{},不然会报初始化操作由case标签跳过
						char optr_ = optr.pop();
						double opndright = opnd.pop();
						double opndleft = opnd.pop();
						opnd.push(calculate(opndleft, optr_, opndright));
						continue;//跳过最后的输入部分。因为这里通过计算产生了新的操作数，所以不需要输入
					}

				case '=':
					optr.pop();
					inte.push(ch);
					show(inte);
					break;
				}//switch的右括号
			}//else的右括号

		input:	cin >> ch;
		}//while的右括号

		if (opnd.isemp())
		{
			cout << endl << "此表达式为空，计算失败" << endl;
			inreset();
			opnd.emplist();
			opndbuff.emplist();
			optr.emplist();
			inte.emplist();
		}
		else
		{
			cout << "\n\n输入结束，下面是计算结果：\n";
			show(inte);//输出表达式（包括最后的“=”）
			cout<<opnd.getop();//在“=”后输出计算结果

			cout << "\n\n下面是计算过程：\n";
			showcalpro();
			cout << endl << endl;

			opnd.emplist();
			opndbuff.emplist();
			optr.emplist();
			inte.emplist();
		}	
	}//evalexpre函数的右括号

}//namespace的右括号
