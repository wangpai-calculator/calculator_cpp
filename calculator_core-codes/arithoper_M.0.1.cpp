
//

#include "arithoper_M.0.1.h"

namespace arithoper
{
	using namespace std;
	stack<double> opnd;//opnd��operand ������
	stack<char> opndbuff;//opndbuff��operand buffer ����Ĳ�������ÿһλ��ֵ
	stack<char> optr;//optr��operator �����
	stack<char> inte;//inte��integration ��ǰ�������ʽ��״̬
	stack<char> ndbfbackup;//ndbfbackup��operand buffer backup �������ı���
	//�˱���ndbfbackup��ʱδ�����ã�ֻ��Ϊ�˱������Ժ�ʹ��

	void inreset()//inreset��input reset ��������
	{
		using namespace std;
		cin.clear();
		while ('\n' != cin.get())
			continue;
	}

	ios_base::fmtflags coutset(int n)//��coutС����������������nλ�����Ҳ�ʹ�ÿ�ѧ������
	{
		using namespace std;
		cout.precision(n);
		auto oldset = cout.setf(ios_base::fixed, ios_base::floatfield);
		return oldset;
	}

	int parematch()//parematch��parentheses match ����ƥ��
	{//�����������ʱ������1�������Ŷ��������ţ�����2��������С�������ţ�����0
		stack<char> pare(optr);
		stack<char> antipare;//antipare��anti pare pare�ķ�ת
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
		if (pare.empty())//������pareΪ�գ�˵�������������
			return true;
		else //������pare��Ϊ�գ�˵����������������������
			return 2;
	}

	char precede(char operleft, char operight) //operleft��operator left ��ߵ��������operight��operator right �ұߵ������
	{//ע�⣺C++������﷨���棬==�����ȼ���&&�ߣ�&&�����ȼ���||��
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

	void show(stack<char>& expression)//չʾ�������ʽ��expression�������Ҫ����ջ���������
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

	void show(stack<char>& expression, stack<double>& medresult, char delimiter)
		// medresult��intermediate result �м���
	{//����delimiter��Ĭ�ϲ�����Ĭ�ϲ���Ϊ#������Ϊ��������ʱ����ζ�ſ��ܽ�������Ķ����ǰ��
		//expression��medresult�������Ҫ����ջ���������

		if (true == expression.empty())
			return;
		else
		{
			coutset(10);
			cout << endl;
			int reorder_exp = expression.size() - 1;//reorder_exp��reverse order expression expression�ĵ������
			int reorder_med = medresult.size() - 1;//reorder_med��reverse order medresult medresult�ĵ������
			int lowbd = 0;//lowbd��lower bound �½�
			if ('#' != delimiter && delimiter == stack_auxCla::index(expression, 0))
			{
				cout << delimiter;//expressionĩβ����иö����������ǰ��
				++lowbd;
			}
			for (; reorder_exp >= lowbd; --reorder_exp)
			{//#Ϊת����
				if ('#' == stack_auxCla::index(expression, reorder_exp))//�������ת��������medresult���Ҳ�����
				{
					cout << stack_auxCla::index(medresult, reorder_med);
					--reorder_med;
				}
				else
					cout << stack_auxCla::index(expression, reorder_exp);
			}
		}
	}

	bool loadopnd()//��opndbuff�ڵĻ�������ת���ɲ�����ѹ�������ջ��loadopnd��load operand װ�ز�����
	{//���㷨ϣ��������ʹ������ķ�����ʹ����ջ�ķ���
		//���ʵ��װ���˲�����������true

		if (true == opndbuff.empty())
			return false;

		double sum = 0;
		int point = stack_auxCla::find(opndbuff, '.');

		ndbfbackup = opndbuff;//���ô��룬����û����
		if (-1 != point)
		{
			stack<char> fraction;
			for (int digit_order = 0; digit_order < point; ++digit_order)
			{//�����С�����֣���Ҫ��ת���ܵ���
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
			opndbuff.pop();//С�������Ѷ�ȡ��ϣ�Ӧȥ���������С����
		}

		for (int digit = 1; !opndbuff.empty(); ++digit)//�������ֿ��Բ��÷�תֱ�ӵ���
		{
			auto top_temp = opndbuff.top();
			opndbuff.pop();
			sum = sum + (top_temp - '0') * pow(10, digit - 1);
		}

		opnd.push(sum);
		return true;
	}

	bool unloadopnd()//��������ջ��Ĳ������˻ػ�������unloadopnd��unload operand �˻ز�����
	{//�˺�����ʱδ��ʹ�ã�ֻ��Ϊ�˱������Ժ�ʹ��

		if (true == opnd.empty())
			return false;

		opnd.pop();
		opndbuff = ndbfbackup;
		return true;
	}

	double calculate(double opndleft, char optr_, double opndright)
	{//�������������������optr_������δ��������������ǳ��������Ժ󽫱���������

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

			/*
			//�˴���Ϊ���ô��룬ʵ��������С�����Ѳ����������������loadopnd��������
			case '.'://�˴�opndright����Ҫ�ǷǸ���
			while (opndright >= 1)
			opndright /= 10;
			return opndleft + opndright;
			*/

		}
	}

	void showcalpro()//showcalpro��show the calculation process չʾ�������
	{//�˺���Ĭ��inte����û���﷨���󣬶���inteû�б����
		//�˺������������opnd��opndbuff��optr������

		if (inte.empty())
		{
			cout << endl << "���ʽ�����ڣ��޷�չʾϸ��" << endl;
			return;
		}

		bool flag = false;//���flagΪtrue��˵������������ִ����һ�μ���

		auto inte_temp = inte;
		if ('=' == inte_temp.top())
			inte_temp.pop();
		show(inte_temp);

		auto intedup = inte;//intedup��inte duplicate inte�ı���,ͬʱ�䵱��������integration
		auto antinte = inte;//antinte��anti inte inte�ķ�ת
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

		auto ch_being_read = antinte.top();//ch_being_read��characters being read ���ڶ�ȡ���ַ�
		antinte.pop();

		while (!antinte.empty() || intedup.empty() || '=' != intedup.top())
		{
			if ('=' == ch_being_read && optr.empty())//���ѭ����=ǰ��Ĳ����Ѵ������
			{
				if (loadopnd())
					intedup.push('#');//'#'Ϊת�������Ժ�����intedup�����˷���˵���˴��ǲ���������Ҫȥ��Ѱ��

				optr.push(ch_being_read);
				intedup.push(ch_being_read);
				continue;/*����Ϊ�����������input���룬��Ϊ��ִ���������飬
						 ˵�����ʽ�ѵ���ͷ������Ҫ�µ������ֱ�ӽ���=����ջ������������ѭ��*/
			}
			else if (isdigit(ch_being_read) || '.' == ch_being_read)//��������ֻ�С����Ļ������������ʱջ
			{
				opndbuff.push(ch_being_read);
			}
			else
			{
				if (loadopnd())
					intedup.push('#');//'#'Ϊת�������Ժ�����intedup�����˷���˵���˴��ǲ���������Ҫȥ��Ѱ��

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
				{//case�����б�����ʼ��һ��Ҫ��{},��Ȼ�ᱨ����ʼ��������case��ǩ�������Ĵ���

							auto optr_ = optr.top();
							optr.pop();
							auto opndright = opnd.top();
							opnd.pop();
							auto opndleft = opnd.top();
							opnd.pop();

							for (int i = 1; i <= 3; ++i)
								intedup.pop();//����������������һ����������������ַ�
							double medresult = calculate(opndleft, optr_, opndright);
							// medresult��intermediate result �м���
							opnd.push(medresult);
							intedup.push('#');//'#'Ϊת�����������˷���˵���˴��ǲ���������Ҫȥ��Ѱ��

							/*���潫��ʾ���ʽ���������������ݡ�����Ҫͬʱ��ʾǰ���Ѿ�����Ĳ���intedup��
							��ǰ�ķ���ch_being_read���Լ���δ��ȡ�ı��ʽantinte*/

							//��Ϊantinte��ʵ������ģ�������show�Ĵ��α�����ջ��������������Ҫ���и��ӵĵ���
							auto poleft = antinte;//poleft��the leftover in positive order ʣ�µ�����Ԫ�� 
							poleft.push(ch_being_read);
							auto reverleft = poleft;//reverleft��the leftover in reverse order ʣ�µ�����Ԫ��
							stack_auxCla::empty_stack(reverleft);
							while (!poleft.empty())
							{
								auto top_temp = poleft.top();
								reverleft.push(top_temp);
								poleft.pop();

							}

							auto fulexp = reverleft;//fulexp��full expression �������ʽ
							/*��Ϊshow�Ĵ�����Ҫջ���򣬶�intedup���������������������ֱ�ӽ�intedup����reverleft�ĺ���*/
							stack_auxCla::stackCat(fulexp, intedup);

							cout << endl;
							show(fulexp, opnd, '=');
							flag = true;//����Ϊ�˸��߳��򣬱���������ִ����һ�μ���	

							continue;/*����Ϊ�����������input���룬��Ϊ��ִ���������飬
									 ˵��ch_being_read�����ȼ��Ƚϵͣ������δ��ջ��δ�������������Բ���Ҫ�µ�����*/
				}
					break;

				case '='://�����������֮��û���������������ֻ��һ��������
				{//case�����б�����ʼ��һ��Ҫ��{},��Ȼ�ᱨ����ʼ��������case��ǩ�������Ĵ���
							 optr.pop();//���������ջ��������


							 auto opnd_temp = intedup.top();

							 /*���ڵ����Ĳ��������ţ���Ϊintedup�е���������ջ������һ����������
							 ����������֮��Ĳ���������������Ҫ�����ģ����Ҫ�ȱ���ò�����*/
							 intedup.pop();

							 intedup.pop();//����������
							 intedup.push(opnd_temp);//��ǰ�汻������ʱ�����Ĳ������ټ���ջ��
				}
					break;

				}//switch��������
			}//else��������


		input:
			ch_being_read = antinte.top();
			antinte.pop();
		}//while��������

		if (!flag)//flag��Ϊ�棬˵���ܿ����û��������һ������Ҫ����ı��ʽ
		{
			inte_temp.push('=');//inte_temp�Ǳ�������ʼ������inte��һ���������ġ�=��֮ǰ����������������������ȥ
			show(inte_temp, opnd, '=');
		}

	}

	void evalexpre()//evalexpre��evaluate expression �Ա��ʽ��ֵ
	{
		cout << "������һ�����ʽ�������ѡ��һ��һ�������룬Ҳ����ѡ��һ�������꣩��\n"
			<< "(��ע������ǡ�/���������ǡ�\\��)\n";

		using namespace std;
		show(inte);
		char characters_entered;
		cin >> characters_entered;
		while (optr.empty() || '=' != optr.top())//��=Ϊ������־
		{
			//�Ⱦ����ܴ��������ܵ����жϵ��﷨����
			if (!cin)
			{
				cout << endl << "���������������룬��Ϊ������" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('.' == characters_entered&& -1 != stack_auxCla::find(opndbuff, '.'))//һ�������������������С����
			{
				cout << endl << "�㲻����һ��������������" << "��" << characters_entered << "��"
					<< "����Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (!opndbuff.empty() && '0' == opndbuff.top() && 1 == opndbuff.size()
				&& '.' != characters_entered)//�����������ֲ���С��1��С�������������λ��Ϊ0
			{
				cout << endl << "�㲻��һ��ʼ������������" << "��" << opndbuff.top() << "��"
					<< "����Ϊ���Զ�ɾ��" << "��" << opndbuff.top() << "��" << "��" << endl;
				opndbuff.pop();//ɾ��һ��ʼ�����0
				inte.pop();
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == characters_entered || '-' == characters_entered
				|| '*' == characters_entered || '/' == characters_entered
				|| '.' == characters_entered) &&
				(inte.empty() || (!isdigit(inte.top()) && inte.top() != ')')))
				//���ǰ��û�е�һ�������������Ǻ���������һ�������ַ�������
			{
				cout << endl << "ע�⣺������" << "��" << characters_entered << "��" << "ǰ�������������һ���Ϸ�������"
					<< "��Ϊ���Զ�ɾ����" << characters_entered << "����" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == characters_entered || '-' == characters_entered
				|| '*' == characters_entered || '/' == characters_entered
				|| '=' == characters_entered || '.' == characters_entered)
				&& !inte.empty() && ('+' == inte.top() || '-' == inte.top() || '*' == inte.top()
				|| '/' == inte.top() || '(' == inte.top() || '.' == inte.top()))//�����������������ַ�������
			{
				cout << endl << "�㲻����" << "��" << inte.top() << "��" << "�ĺ�������"
					<< "��" << characters_entered << "��" << "����Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('(' == characters_entered && !inte.empty() && isdigit(inte.top())
				&& '.' == characters_entered)//���������ǰ�������֡�С���㣬����
			{
				cout << endl << "������ǰ�治�������֣���Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (')' == characters_entered)
			{
				if (!inte.empty() && (('+' == inte.top() || '-' == inte.top()
					|| '*' == inte.top() || '/' == inte.top())))//���������ǰ�������������������
				{
					cout << endl << "������ǰ�治�����������������Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
					inreset();
					show(inte);
					goto input;
				}
				if (!inte.empty() && '(' == inte.top())
				{
					cout << endl << "��һ��������ʲôҲû�У���Ϊ���Զ�ɾ����һ�����ţ�" << endl;
					inreset();
					optr.pop();
					inte.pop();
					show(inte);
					goto input;
				}

				optr.push(characters_entered);//���¾�Ҫ�������ƥ������⣬������ƥ���������ջ���ܼ��
				int matchresult = parematch();//����ƥ����
				optr.pop();//�ո�ֻ��Ϊ�˼������ƥ�����ջ���������������ּ��ĸ�����
				if (false == matchresult)//��������Ų�ƥ�䣬����
				{
					cout << endl << "�����Ų�ƥ�䣬��Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
					inreset();
					show(inte);
					goto input;
				}
			}
			if ('=' == characters_entered)
			if (2 == parematch())//���׼����������ʱ��������������Ŷ��������ţ�����
			{
				cout << endl << "�����Ų��ܶ��������ţ���Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}

			//���ڵ���������µĴ���
			//ע�⣺������if else�ж�����������ŵľ���������䣬��������Ĵ�����п��ܿ���ʡ�ԡ�goto input������
			if ('=' == characters_entered && optr.empty())//���ѭ����=ǰ��Ĳ����Ѵ�����ϣ���ô���ｫ����β�������Խ���ѭ��
			{
				loadopnd();
				optr.push(characters_entered);//�˴��á�=����ջ��Ŀ����Ϊ���´��ܽ���ѭ��
				inte.push(characters_entered);
				show(inte);
				continue;//�����������벿�֡���Ϊ���ڲ���Ҫ������
			}
			else if (isdigit(characters_entered) || '.' == characters_entered)//��������ֻ�С����Ļ������������ʱջ
			{
				opndbuff.push(characters_entered);
				inte.push(characters_entered);
				show(inte);
			}
			else
			{
				loadopnd();
				if (optr.empty())//��������ջ��ʲôҲû�У����������ֱ����ջ
				{
					optr.push(characters_entered);
					inte.push(characters_entered);
					show(inte);
					goto input;//��Ϊ����else�黹��ʣ���switch�飬�������ﲻ��ʡ�Դ����
				}
				switch (precede(optr.top(), characters_entered))
				{/*��switch�������󣬾�ֱ����else�Ľ������֣������߽�����͵������벿�֣�
				 �������break�����ܿ��������goto input��*/

				case '<':
					optr.push(characters_entered);
					inte.push(characters_entered);
					show(inte);
					break;

				case '>':
					if ('(' != characters_entered && '/' == optr.top() && !opnd.empty() && 0 == opnd.top())//�����0��Ϊ����
					{
						if (!inte.empty() && '0' == inte.top())
						{
							cout << endl << "��Ǹ��0��������������Ϊ���Զ�ɾ����" << endl;
							inreset();
							opnd.pop();
							inte.pop();
							show(inte);
							break;
						}
						else
						{
							cout << endl << "�м��е���ʽ�ó���Ϊ0�����μ���ʧ�ܣ�����Ϊ�㶨λ�������룺" << endl;
							inreset();
							stack_auxCla::empty_stack(opnd);
							stack_auxCla::empty_stack(opndbuff);
							stack_auxCla::empty_stack(optr);
							stack_auxCla::empty_stack(inte);
							break;
						}
					}
					{//case�����б�����ʼ��һ��Ҫ��{},��Ȼ�ᱨ��ʼ��������case��ǩ����

						auto optr_ = optr.top();
						optr.pop();
						auto opndright = opnd.top();
						opnd.pop();
						auto opndleft = opnd.top();
						opnd.pop();
						opnd.push(calculate(opndleft, optr_, opndright));
						continue;//�����������벿�֡���Ϊ����ͨ������������µĲ����������Բ���Ҫ����
					}
					break;

				case '=':
					optr.pop();
					inte.push(characters_entered);
					show(inte);
					break;

				}//switch��������
			}//else��������

		input:	cin >> characters_entered;
		}//while��������

		if (opnd.empty())
		{
			cout << endl << "�˱��ʽΪ�գ�����ʧ��" << endl;
			inreset();

			stack_auxCla::empty_stack(opnd);
			stack_auxCla::empty_stack(opndbuff);
			stack_auxCla::empty_stack(optr);
			stack_auxCla::empty_stack(inte);
		}
		else
		{
			cout << "\n\n��������������Ǽ�������\n";
			show(inte);//������ʽ���������ġ�=����
			cout << opnd.top();//�ڡ�=�������������

			cout << "\n\n�����Ǽ�����̣�\n";
			showcalpro();
			cout << endl << endl;

			inreset();

			stack_auxCla::empty_stack(opnd);
			stack_auxCla::empty_stack(opndbuff);
			stack_auxCla::empty_stack(optr);
			stack_auxCla::empty_stack(inte);
		}
	}//evalexpre������������

}//namespace��������
