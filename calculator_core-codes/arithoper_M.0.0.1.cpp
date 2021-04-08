
//

#include "arithoper_M.0.0.1.h"

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

	void show(WangpaiStack<char>& expression, WangpaiStack<double>& medresult, char delimiter)
		// medresult��intermediate result �м���
	{//����delimiter��Ĭ�ϲ�����Ĭ�ϲ���Ϊ#������Ϊ��������ʱ����ζ�ſ��ܽ�������Ķ����ǰ��
	 //expression��medresult�������Ҫ����ջ���������

		if (true == expression.isemp())
			return;
		else
		{
			coutset(10);
			cout << endl;
			int reorder_exp = expression.count();//reorder_exp��reverse order expression expression�ĵ������
			int reorder_med = medresult.count();//reorder_med��reverse order medresult medresult�ĵ������
			int lowbd = 1;//lowbd��lower bound �½�
			if ('#' != delimiter && delimiter == expression[1])
			{
				cout << delimiter;//expressionĩβ����иö����������ǰ��
				++lowbd;
			}
			for (; reorder_exp >= lowbd; --reorder_exp)
			{//#Ϊת����
				if ('#' == expression[reorder_exp])//�������ת��������medresult���Ҳ�����
				{
					cout << medresult[reorder_med];
					--reorder_med;
				}
				else
					cout << expression[reorder_exp];
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
			sum = sum + (opndbuff.pop() - '0') * pow(10, digit - 1);

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
		antinte.deleteall();
		while (!intedup.isemp())
			antinte.push(intedup.pop());
		opnd.deleteall();
		opndbuff.deleteall();
		optr.deleteall();
		char ch_being_read = antinte.pop();	//ch_being_read��characters being read ���ڶ�ȡ���ַ�

		while (!antinte.isemp() || intedup.isemp() || '=' != intedup.getop())
		{
			if ('=' == ch_being_read && optr.isemp())//���ѭ����=ǰ��Ĳ����Ѵ������
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

				if (optr.isemp())
				{
					optr.push(ch_being_read);
					intedup.push(ch_being_read);
					goto input;
				}
				switch (precede(optr.getop(), ch_being_read))
				{
				case '<':
					optr.push(ch_being_read);
					intedup.push(ch_being_read);
					break;

				case '>':
				{//case�����б�����ʼ��һ��Ҫ��{},��Ȼ�ᱨ����ʼ��������case��ǩ�������Ĵ���
							char optr_ = optr.pop();
							double opndright = opnd.pop();
							double opndleft = opnd.pop();
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
							reverleft.deleteall();
							while (!poleft.isemp())
								reverleft.push(poleft.pop());
							auto fulexp = reverleft;//fulexp��full expression �������ʽ
							fulexp += intedup;/*��Ϊshow�Ĵ�����Ҫջ���򣬶�intedup�����������
											  ����������ֱ�ӽ�intedup����reverleft�ĺ���*/

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
							auto opnd_temp = intedup.pop();/*���ڵ����Ĳ��������ţ���Ϊintedup�е���������ջ������һ����������
														   ����������֮��Ĳ���������������Ҫ�����ģ����Ҫ�ȱ���ò�����*/
							intedup.pop();//����������
							intedup.push(opnd_temp);//��ǰ�汻������ʱ�����Ĳ������ټ���ջ��
				}
					break;

				}//switch��������
			}//else��������

		input:	ch_being_read = antinte.pop();
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
			if ('.' == characters_entered&& -1 != opndbuff.search('.'))//һ�������������������С����
			{
				cout << endl << "�㲻����һ��������������" << "��" << characters_entered << "��"
					<< "����Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (!opndbuff.isemp() && '0' == opndbuff.getop() && 1 == opndbuff.count()
				&& '.' != characters_entered)//�����������ֲ���С��1��С�������������λ��Ϊ0
			{
				cout << endl << "�㲻��һ��ʼ������������" << "��" << opndbuff.getop() << "��"
					<< "����Ϊ���Զ�ɾ��" << "��" << opndbuff.getop() << "��" << "��" << endl;
				opndbuff.pop();//ɾ��һ��ʼ�����0
				inte.pop();
				inreset();
				show(inte);
				goto input;
			}
			if (('+' == characters_entered || '-' == characters_entered
				|| '*' == characters_entered || '/' == characters_entered
				|| '.' == characters_entered) &&
				(inte.isemp() || (!isdigit(inte.getop()) && inte.getop() != ')')))
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
				&& !inte.isemp() && ('+' == inte.getop() || '-' == inte.getop() || '*' == inte.getop()
				|| '/' == inte.getop() || '(' == inte.getop() || '.' == inte.getop()))//�����������������ַ�������
			{
				cout << endl << "�㲻����" << "��" << inte.getop() << "��" << "�ĺ�������"
					<< "��" << characters_entered << "��" << "����Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if ('(' == characters_entered && !inte.isemp() && isdigit(inte.getop())
				&& '.' == characters_entered)//���������ǰ�������֡�С���㣬����
			{
				cout << endl << "������ǰ�治�������֣���Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
				inreset();
				show(inte);
				goto input;
			}
			if (')' == characters_entered)
			{
				if (!inte.isemp() && (('+' == inte.getop() || '-' == inte.getop()
					|| '*' == inte.getop() || '/' == inte.getop())))//���������ǰ�������������������
				{
					cout << endl << "������ǰ�治�����������������Ϊ���Զ�ɾ��" << "��" << characters_entered << "��" << "��" << endl;
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
			if ('=' == characters_entered && optr.isemp())//���ѭ����=ǰ��Ĳ����Ѵ�����ϣ���ô���ｫ����β�������Խ���ѭ��
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
				if (optr.isemp())//��������ջ��ʲôҲû�У����������ֱ����ջ
				{
					optr.push(characters_entered);
					inte.push(characters_entered);
					show(inte);
					goto input;//��Ϊ����else�黹��ʣ���switch�飬�������ﲻ��ʡ�Դ����
				}
				switch (precede(optr.getop(), characters_entered))
				{/*��switch�������󣬾�ֱ����else�Ľ������֣������߽�����͵������벿�֣�
				 �������break�����ܿ��������goto input��*/

				case '<':
					optr.push(characters_entered);
					inte.push(characters_entered);
					show(inte);
					break;

				case '>':
					if ('(' != characters_entered && '/' == optr.getop() && !opnd.isemp() && 0 == opnd.getop())//�����0��Ϊ����
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
							opnd.deleteall();
							opndbuff.deleteall();
							optr.deleteall();
							inte.deleteall();
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

		if (opnd.isemp())
		{
			cout << endl << "�˱��ʽΪ�գ�����ʧ��" << endl;
			inreset();
			opnd.deleteall();
			opndbuff.deleteall();
			optr.deleteall();
			inte.deleteall();
		}
		else
		{
			cout << "\n\n��������������Ǽ�������\n";
			show(inte);//������ʽ���������ġ�=����
			cout << opnd.getop();//�ڡ�=�������������

			cout << "\n\n�����Ǽ�����̣�\n";
			showcalpro();
			cout << endl << endl;

			opnd.deleteall();
			opndbuff.deleteall();
			optr.deleteall();
			inte.deleteall();
		}
	}//evalexpre������������

}//namespace��������
