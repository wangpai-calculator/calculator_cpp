
//


#include "arithoper.h"

namespace arithoper
{
	using namespace std;
	WangpaiStack<double> opnd;//opnd��operand ������
	WangpaiStack<char> opndbuff;//opndbuff��operand buffer ����Ĳ�������ÿһλ��ֵ
	WangpaiStack<char> optr;//optr��operator �����
	WangpaiStack<char> inte;//inte��integration ��ǰ�������ʽ��״̬
	WangpaiStack<char> ndbfbackup;//ndbfbackup��operand buffer backup �������ı���
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
		WangpaiStack<char> pare(optr);
		WangpaiStack<char> antipare;//antipare��anti pare pare�ķ�ת
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
		if (pare.isemp())//������pareΪ�գ�˵�������������
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

	void show(WangpaiStack<char>& expression)//չʾ�������ʽ��expression�������Ҫ����ջ���������
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

	void show2(WangpaiStack<char>& expression, WangpaiStack<double>& medresult, char delimiter)// medresult��intermediate result �м���
	{//����delimiter��Ĭ�ϲ�����Ĭ�ϲ���Ϊ#������Ϊ��������ʱ����ζ�ſ��ܽ�������Ķ����ǰ��
	//expression��medresult�������Ҫ����ջ���������
		if (true == expression.isemp())
			return;
		else
		{
			coutset(10);
			cout << endl;
			int order1 = expression.count();
			int order2 = medresult.count();
			int lowbo = 1;//lowbo��lower bound �½�
			if ('#' != delimiter && delimiter == expression[1])
			{
				cout << delimiter;//expressionĩβ����иö����������ǰ��
				++lowbo;
			}
			for (; order1 >= lowbo; --order1)
			{//#Ϊת����
				if ('#' == expression[order1])//�������ת��������medresult���Ҳ�����
				{
					cout << medresult[order2];
					--order2;
				}
				else
				cout << expression[order1];
			}
		}
	}

	bool loadopnd()//��opndbuff�ڵĻ�������ת���ɲ�����ѹ�������ջ��loadopnd��load operand װ�ز�����
	{//���㷨ϣ��������ʹ������ķ�����ʹ����ջ�ķ���
	 //���ʵ��װ���˲�����������true
		if (true == opndbuff.isemp())
			return false;

		double sum = 0;
		int length = opndbuff.count();
		int point = opndbuff.search('.');
		ndbfbackup = opndbuff;//���ô��룬����û����
		if (-1 != point)
		{
			WangpaiStack<char> fraction;
			for (int digit = 1; digit < point; ++digit)
				fraction.push(opndbuff.pop());//�����С�����֣���Ҫ��ת���ܵ���
			for (int digit = 1; !fraction.isemp(); ++digit)
				sum = sum + (fraction.pop() - '0') / pow(10, digit);
			opndbuff.pop();//С�������Ѷ�ȡ��ϣ�Ӧȥ���������С����
		}

		for (int digit = 1; !opndbuff.isemp(); ++digit)//�������ֿ��Բ��÷�תֱ�ӵ���
			sum = sum + (opndbuff.pop() - '0') * pow(10, digit-1);

		opnd.push(sum);
		return true;
	}

	bool unloadopnd()//��������ջ��Ĳ������˻ػ�������unloadopnd��unload operand �˻ز�����
	{//�˺�����ʱδ��ʹ�ã�ֻ��Ϊ�˱������Ժ�ʹ��
		if (true == opnd.isemp())
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
		case '.'://�˴�opndright����Ҫ�ǷǸ���
			while (opndright >= 1)
				opndright /= 10;
			return opndleft + opndright;
		}
	}

	void showcalpro()//showcalpro��show the calculation process չʾ�������
	{//�˺���Ĭ��inte����û���﷨���󣬶���inteû�б����
     //�˺������������opnd��opndbuff��optr������
		if (inte.isemp())
		{
			cout << endl << "���ʽ�����ڣ��޷�չʾϸ��" << endl;
			return;
		}

		bool flag = false;//���flagΪtrue��˵������������ִ����һ�μ���

		auto inte_temp = inte;
		if ('=' == inte_temp.getop())
			inte_temp.pop();
		show(inte_temp);

		auto intedup = inte;//intedup��inte duplicate inte�ı���,ͬʱ�䵱��������integration
		auto antinte = inte;//antinte��anti inte inte�ķ�ת
		antinte.emplist();
		while (!intedup.isemp())
			antinte.push(intedup.pop());
		opnd.emplist();
		opndbuff.emplist();
		optr.emplist();
		char ch = antinte.pop();
		
		while (!antinte.isemp() || intedup.isemp() || '='!=intedup.getop())
		{
			if ('=' == ch && optr.isemp())//���ѭ����=ǰ��Ĳ����Ѵ������
			{
				if (loadopnd())
					intedup.push('#');//'#'Ϊת�������Ժ�����intedup�����˷���˵���˴��ǲ���������Ҫȥ��Ѱ��

				optr.push(ch);
				intedup.push(ch);
				continue;/*����Ϊ�����������input���룬��Ϊ��ִ���������飬
						 ˵�����ʽ�ѵ���ͷ������Ҫ�µ������ֱ�ӽ���=����ջ������������ѭ��*/
			}
			else if (isdigit(ch))//��������ֵĻ������������ʱջ
			{
				opndbuff.push(ch);
			}
			else
			{
				if (loadopnd())
					intedup.push('#');//'#'Ϊת�������Ժ�����intedup�����˷���˵���˴��ǲ���������Ҫȥ��Ѱ��

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
					{//case�����б�����ʼ��һ��Ҫ��{},��Ȼ�ᱨ��ʼ��������case��ǩ����
						bool pointflag = false;//���������С���㵼�µ����㣬�˱�־����true
						char optr_ = optr.pop();
						if ('.' == optr_)
							pointflag = true;
						double opndright = opnd.pop();
						double opndleft = opnd.pop();
						for (int i = 1; i <= 3; ++i)
							intedup.pop();//����������������һ����������������ַ�
						double medresult = calculate(opndleft, optr_, opndright);
						// medresult��intermediate result �м���
						opnd.push(medresult);
						intedup.push('#');//'#'Ϊת�����������˷���˵���˴��ǲ���������Ҫȥ��Ѱ��
								
						if (!pointflag)//�����С���㵼�µ����㣬�����뱾�����һ�����㡢������if���������
						{   //���潫��ʾ���ʽ���������������ݡ�����Ҫͬʱ��ʾǰ���Ѿ�����Ĳ���intedup����ǰ�ķ���ch���Լ���δ��ȡ�ı��ʽantinte
							
							//��Ϊantinte��ʵ������ģ�������show2�Ĵ��α�����ջ��������������Ҫ���и��ӵĵ���
							auto poleft = antinte;//poleft��the leftover in positive order ʣ�µ�����Ԫ�� 
							poleft.push(ch);
							auto reverleft = poleft;//reverleft��the leftover in reverse order ʣ�µ�����Ԫ��
							reverleft.emplist();
							while (!poleft.isemp())
								reverleft.push(poleft.pop());
							auto fulexp = reverleft;//fulexp��full expression �������ʽ
							fulexp += intedup;//��Ϊshow2�Ĵ�����Ҫջ���򣬶�intedup���������������������ֱ�ӽ�intedup����reverleft�ĺ���

							cout << endl;
							show2(fulexp, opnd, '=');
							flag = true;//����Ϊ�˸��߳��򣬱���������ִ����һ�μ���
						}
						continue;/*����Ϊ�����������input���룬��Ϊ��ִ���������飬
								 ˵��ch�����ȼ��Ƚϵͣ������δ��ջ��δ�������������Բ���Ҫ�µ�����*/
					}
				case '=':
					{//�����������֮��û���������������ֻ��һ��������
						optr.pop();//���������ջ��������
						auto opnd_temp = intedup.pop();/*���ڵ����Ĳ��������ţ���Ϊintedup�е���������ջ������һ����������
													   ����������֮��Ĳ���������������Ҫ�����ģ����Ҫ�ȱ���ò�����*/
						intedup.pop();//����������
						intedup.push(opnd_temp);//��ǰ�汻������ʱ�����Ĳ������ټ���ջ��
					}	
					break;
				}//switch��������
			}//else��������

		input:	ch = antinte.pop();
		}//while��������

		if (!flag)//flag��Ϊ�棬˵���ܿ����û��������һ������Ҫ����ı��ʽ
		{
			inte_temp.push('=');//inte_temp�Ǳ�������ʼ������inte��һ���������ġ�=��֮ǰ����������������������ȥ
			show2(inte_temp, opnd, '=');
		}

	}

	void evalexpre()//evalexpre��evaluate expression �Ա��ʽ��ֵ
	{
		cout << "������һ�����ʽ�������ѡ��һ��һ�������룬Ҳ����ѡ��һ�������꣩��\n"
			<< "(��ע������ǡ�/���������ǡ�\\��)\n";

		using namespace std;
		show(inte);
		char ch;
		cin >> ch;
		while (optr.isemp() || '=' != optr.getop())//��=Ϊ������־
		{
			//�Ⱦ����ܴ��������ܵ����жϵ��﷨����
			if (!cin)
			{
				cout << endl << "���������������룬��Ϊ������" << endl;
				inreset();
				show(inte);
				goto input;
			}	
			if (('.' == ch) && !optr.isemp() && ('.' == optr.getop()))//һ�������������������С����
			{
				cout << endl << "�㲻����һ��������������" << "��" << ch << "��"
					<< "����Ϊ���Զ�ɾ��" << "��" << ch << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == ch || '-' == ch || '*' == ch || '/' == ch || '.' == ch) &&
				(inte.isemp() || (!isdigit(inte.getop()) && inte.getop() != ')')))
				//���ǰ��û�е�һ�������������Ǻ���������һ�������ַ�������
			{
				cout << endl << "ע�⣺������" << "��" << ch << "��" << "ǰ�������������һ���Ϸ�������"
					<< "��Ϊ���Զ�ɾ����" << ch << "����" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == ch || '-' == ch || '*' == ch || '/' == ch || '=' == ch || '.' == ch)
				&& !inte.isemp() && ('+' == inte.getop() || '-' == inte.getop() || '*' == inte.getop()
				|| '/' == inte.getop() || '(' == inte.getop() || '.' == inte.getop()))//�����������������ַ�������
			{
				cout << endl << "�㲻����" << "��" << inte.getop() << "��" << "�ĺ�������"
					<< "��" << ch << "��" << "����Ϊ���Զ�ɾ��" << "��" << ch << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('(' == ch && !inte.isemp() && isdigit(inte.getop()) && ('.' == ch))//���������ǰ�������֡�С���㣬����
			{
				cout << endl << "������ǰ�治�������֣���Ϊ���Զ�ɾ��" << "��" << ch << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (')' == ch)
			{
				if (!inte.isemp() && (('+' == inte.getop() || '-' == inte.getop()
					|| '*' == inte.getop() || '/' == inte.getop())))//���������ǰ�������������������
				{
					cout << endl << "������ǰ�治�����������������Ϊ���Զ�ɾ��" << "��" << ch << "��" << "��" << endl;
					inreset();
					show(inte);
					goto input;
				}
				if (!inte.isemp() && '(' == inte.getop())
				{
					cout << endl << "��һ��������ʲôҲû�У���Ϊ���Զ�ɾ����һ�����ţ�" << endl;
					inreset();
					optr.pop();
					inte.pop();
					show(inte);
					goto input;
				}

				optr.push(ch);//���¾�Ҫ�������ƥ������⣬������ƥ���������ջ���ܼ��
				int matchresult = parematch();//����ƥ����
				optr.pop();//�ո�ֻ��Ϊ�˼������ƥ�����ջ���������������ּ��ĸ�����
				if (false == matchresult)//��������Ų�ƥ�䣬����
				{
					cout << endl << "�����Ų�ƥ�䣬��Ϊ���Զ�ɾ��" << "��" << ch << "��" << "��" << endl;
					inreset();
					show(inte);
					goto input;
				}
			}
			if ('=' == ch)
			if (2 == parematch())//���׼����������ʱ��������������Ŷ��������ţ�����
			{
				cout << endl << "�����Ų��ܶ��������ţ���Ϊ���Զ�ɾ��" << "��" << ch << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
	
			//���ڵ���������µĴ���
			//ע�⣺������if else�ж�����������ŵľ���������䣬��������Ĵ�����п��ܿ���ʡ�ԡ�goto input������
			if ('=' == ch && optr.isemp())//���ѭ����=ǰ��Ĳ����Ѵ�����ϣ���ô���ｫ����β�������Խ���ѭ��
			{
				loadopnd();
				optr.push(ch);//�˴��á�=����ջ��Ŀ����Ϊ���´��ܽ���ѭ��
				inte.push(ch);
				show(inte);
				continue;//�����������벿�֡���Ϊ���ڲ���Ҫ������
			}
			else if (isdigit(ch))//��������ֵĻ������������ʱջ
			{
				opndbuff.push(ch);
				inte.push(ch);
				show(inte);
			}
			else
			{
				loadopnd();
				if (optr.isemp())//��������ջ��ʲôҲû�У����������ֱ����ջ
				{
					optr.push(ch);
					inte.push(ch);
					show(inte);
					goto input;//��Ϊ����else�黹��ʣ���switch�飬�������ﲻ��ʡ�Դ����
				}
				switch (precede(optr.getop(), ch))
				{/*��switch�������󣬾�ֱ����else�Ľ������֣������߽�����͵������벿�֣��������break�����ܿ��������goto input��*/

				case '<':
					optr.push(ch);
					inte.push(ch);
					show(inte);
					break;
				case '>':
					if ('(' != ch && '/' == optr.getop() && !opnd.isemp() && 0 == opnd.getop())//�����0��Ϊ����
					{
						if (!inte.isemp() && '0' == inte.getop())
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
							opnd.emplist();
							opndbuff.emplist(); 
							optr.emplist(); 
							inte.emplist();
							break;
						}
					}
					{//case�����б�����ʼ��һ��Ҫ��{},��Ȼ�ᱨ��ʼ��������case��ǩ����
						char optr_ = optr.pop();
						double opndright = opnd.pop();
						double opndleft = opnd.pop();
						opnd.push(calculate(opndleft, optr_, opndright));
						continue;//�����������벿�֡���Ϊ����ͨ������������µĲ����������Բ���Ҫ����
					}

				case '=':
					optr.pop();
					inte.push(ch);
					show(inte);
					break;
				}//switch��������
			}//else��������

		input:	cin >> ch;
		}//while��������

		if (opnd.isemp())
		{
			cout << endl << "�˱��ʽΪ�գ�����ʧ��" << endl;
			inreset();
			opnd.emplist();
			opndbuff.emplist();
			optr.emplist();
			inte.emplist();
		}
		else
		{
			cout << "\n\n��������������Ǽ�������\n";
			show(inte);//������ʽ���������ġ�=����
			cout<<opnd.getop();//�ڡ�=�������������

			cout << "\n\n�����Ǽ�����̣�\n";
			showcalpro();
			cout << endl << endl;

			opnd.emplist();
			opndbuff.emplist();
			optr.emplist();
			inte.emplist();
		}	
	}//evalexpre������������

}//namespace��������
