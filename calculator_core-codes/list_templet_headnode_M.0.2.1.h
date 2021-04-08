
//��Ϊ��ͷ��������ͷ�ļ�

#pragma once

#include <cstring>
#include <string> 
#include <climits>
#include <iostream>

using namespace std;

template <typename DataType>
class WangpaiList
{
protected:

	class NodeInfo //NodeInfo��node information �����Ϣ
	{//ע�⣬�ڲ���ĳ�Ա�������ⲿ�࣬���Ҫʹ���ڲ���ĳ�Ա����ֱ�Ӷ����ڲ������
	private:
#ifndef defaultname
/*����ʹ���˺꣬����ζ��ǰ�治��ʹ��ͬ���ĺꡣ
���ʹ����ͬ���ĺ꣬�����һ���ִ��뽫��ȥ�����⽫���±����������Ժܺõ������û�*/
#define defaultname "��Ϊ����Ľ����Ϣ"

		string listname;

	public:

		//�˷������䵱Ĭ�Ϲ��캯����ע�⣬Ĭ�Ϲ��캯��ֻ����һ��
		//forname��formal parameter name �β�name
		explicit NodeInfo(const string forname = defaultname) :listname(forname){}
		explicit NodeInfo(const char* const forname) :listname(string(forname)){}


		friend ostream& operator<<(ostream& os, NodeInfo& info)//info��information ��Ϣ
		{
			cout << "������Ϣ��" << info.listname;
			return os;
		}

#undef defaultname
//����Ĵ��벻��Ҫ�˺꣬��������ȥ��ǰ��ĺ궨��
#endif
	};

	template <typename DataType_>
	class Node
	{
	public:
		DataType_ data;
		Node* next;

	public:

		//�˷������䵱Ĭ�Ϲ��캯����ע�⣬Ĭ�Ϲ��캯��ֻ����һ��
		explicit Node(Node* const pnt = nullptr) :next(pnt){}
		explicit Node(const DataType_& rdata, Node* const pnt = nullptr) :data(rdata), next(pnt){}
		explicit Node(const Node& other) = delete;//��㲻����ֱ�Ӹ���

		Node& operator=(const Node& other) = delete;//��㲻����ֱ�Ӹ���
	};

	/*-----------------���ݳ�Ա---------------------------*/
protected:

#ifndef defaultname
	/*����ʹ���˺꣬����ζ��ǰ�治��ʹ��ͬ���ĺꡣ
	���ʹ����ͬ���ĺ꣬�����һ���ִ��뽫��ȥ�����⽫���±����������Ժܺõ������û�*/
#define defaultname "����������ͷ���"

	const int MAX_LENGTH;

	NodeInfo information;
	Node<DataType>* const head;//һ����ʼ����Ͳ��ܱ��޸ģ���ͷ���ĵ�ַ��������������������ж�����仯
	int length;//����ĳ���Ϊ����Ԫ�صĸ������˸�������ͷ��㡣

	/*-----------------���ݳ�Ա---------------------------*/

protected:

	virtual WangpaiList& operator+(const DataType& data)
	{
		inserdata(data, this->length + 1);
		return *this;
	}

	virtual WangpaiList& operator+(const WangpaiList& other)//ע��˺������뱣֤other�������ݵ�������
	{
		auto pt_this = searchp(this->length);
		auto pt_other = other.head->next;//ָ��other�����һ����Ҫ����������

		while (pt_other)//��ָ�벻Ϊ�գ�˵����ǰָ��pt_otherָ����һ��ʵ�ʵ����ݣ���ô��ִ������Ŀ���
		{
			pt_this->next = new Node<DataType>(pt_other->data);
			pt_this = pt_this->next;
			pt_other = pt_other->next;
		}
		pt_this->next = nullptr;
		this->length += other.length;
		return *this;
	}

	virtual Node<DataType>* searchp(int seqnum)//searchp��search pointer ����ָ�������
	{
		if (seqnum<0 || seqnum>this->length)
			throw "���Ժ��� virtual Node<DataType>* WangpaiList<DataType>::searchp(int seqnum) �ķ���:\n"
			"   ���ṩ�����seqnum������\n\n";//����ʹ���쳣ʵ���Ȳ�����
		else
		{
			auto pt = this->head;
			int order = 0;//order�����ָ�룬pt��ʵ��ָ�룬�����������ָ���ֵ����������
			for (;
				order != seqnum;//��Ϊ����ִ�е���һ��ʱ��orderһ��������seqnum�����Ե�order��������ʱ��һ�����ܵ���seqnum
				pt = pt->next, ++order)
				continue;
			return pt;
		}
	}

public:

	//�˷������䵱Ĭ�Ϲ��캯����ע�⣬Ĭ�Ϲ��캯��ֻ����һ��
	explicit WangpaiList(const string listname = defaultname, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(listname), head(new Node<DataType>), length(0){}

	explicit WangpaiList(const char* const listname, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(string(listname)), head(new Node<DataType>), length(0){}

	explicit WangpaiList(const DataType& data, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(defaultname), head(new Node<DataType>), length(0)
	{
		*this = data;//����Ĺ������� =��������غ��� �����
	}

	explicit WangpaiList(const WangpaiList& other, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(other.information), head(new Node<DataType>), length(0)
	{
		*this = other;//����Ĺ������� =��������غ��� �����
	}

	virtual ~WangpaiList()//һ�������˸ú�����ͷ��㶼�ᱻɾ��,����һ��������󣬴˺���ֻ��������һ�Σ���˲������ֶ�����
	{
		deleteall();
		delete this->head;
	}

	virtual WangpaiList& operator=(const DataType& data)
	{
		deleteall();
		return *this += data;
	}

	virtual WangpaiList& operator=(const WangpaiList& other)
	{
		deleteall();
		return *this += other;
	}

	virtual WangpaiList& operator+=(const DataType& data)
	{
		return *this + data;
	}

	virtual WangpaiList& operator+=(const WangpaiList& other)
	{
		return *this + other;
	}

	virtual DataType& operator[](int seqnum)
	{
		try//����try��catch����ֻ��һ����䣬Ҳ����ʹ�ô�����
		{
			return searchp(seqnum)->data;

		}
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{
			throw "���Ժ��� virtual DataType& WangpaiList<DataType>::operator[](int seqnum) �ķ���:\n"
				"   ���ṩ�����seqnum������\n\n";
		}
	}

	virtual WangpaiList& deleteall()//deleteall��delete all	ɾ������
	{//ע����øú�������ɾ��ͷ��㣬ͷ�������������
		for (; this->length > 0;)//����this->length�䵱���ָ�룬ָ���������һ��Ԫ�ء�
			deldata(this->length);
		return *this;
	}

	virtual bool isemp()//isemp��is empty �ж��Ƿ�Ϊ��
	{
		return 0 == this->length;
	}

	virtual bool isful()//isful��is full �ж��Ƿ�Ϊ��
	{
		return MAX_LENGTH == this->length;
	}

	virtual int count()
	{
		return this->length;
	}

	virtual WangpaiList& showall()
	{
		cout << *this;
		return *this;
	}

	//Ѱ��data�������״��ҵ������
	virtual int search(const DataType& data)                               
	{//�˺�����ʵ����ҪDataType�Ѿ�������==��������غ���
	 //�˺����������Ϊ1��Ԫ�ؿ�ʼ������Ҳ����˵��������ͷ��㣬��Ϊͷ��㲻�������ݣ�
	 //����涨Ԫ�ص���ž��ǵ�ɾ���������Ԫ��ʱ����ĳ��ȡ����磺ͷ�������Ϊ0

		/*//������һ���汾�Ķ��壬һ�ֶ���������������������İ汾
		auto& myself = *this;
		for (int order = 1; order <= this->length; ++order)//ÿ��ѭ��������orderָ�������һ���Ƚ϶�������
		if (data == myself[order])
			return order;
		return -1;
		*/

		return this->search(data, 1, this->length);
	}

	//��begin-end֮��Ѱ��data�������״��ҵ������
	virtual int search(const DataType& data, int begin, int end)
	{//�˺�����ʵ����ҪDataType�Ѿ�������==��������غ���
	 //����涨Ԫ�ص���ž��ǵ�ɾ���������Ԫ��ʱ����ĳ��ȡ����磺ͷ�������Ϊ0

	 /*֮���Զ��ⶨ��������ص�search������������֮ǰ��һ��������search����������������Ĭ�ϲ�����
	 ����Ϊ������Ա��ΪĬ�ϲ���ʹ��Ҫ��̬��Ա�������end����ʹ��this->length��ΪĬ�ϲ���*/

		if (begin < 1 || end > this->length)
			return -1;

		auto& myself = *this;
		for (int order = begin; order <= end; ++order)//ÿ��ѭ��������orderָ�������һ���Ƚ϶�������
		if (data == myself[order])
			return order;

		return -1;
	}

	virtual int locate(const Node<DataType>* pt)
	{
		for (int order = 0; order <= this->length;++order)//ÿ��ѭ��������orderָ�������һ���Ƚ϶�������
		if (pt == searchp(order))
				return order;
		return -1;
	}


	virtual bool inserdata(const DataType& data, int seqnum)//inserdata��insert data ��������
	{//���ǽ����ݲ����seqnum����㣬����������ݱ���˵�seqnum�������������ݣ�ԭ����Ӧλ�õĽ����ơ�
		try
		{
			auto pt = this->searchp(seqnum - 1);//����ѡ���õ�ǰָ��ptָ��Ҫ����λ�õ���һ��λ�ã�����Ϊ�˱����޸���һ������next
			pt->next = new Node<DataType>(data, pt->next);
			++this->length;
			return true;
		}
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{//����catch����ֻ��һ����䣬Ҳ����ʹ�ô�����
			return false;
		}
	}

	virtual WangpaiList& forinser(const DataType& data)//forinser��forward insertion ǰ�巨
	{
		this->inserdata(data, 1);
		return *this;
	}

	virtual WangpaiList& backinser(const DataType& data)//backinser��backward insertion ��巨
	{
		this->inserdata(data, this->length + 1);
		return *this;
	}

	virtual bool deldata(int seqnum)//deldata��delete data ɾ������
	{//ע�������������ɾ��ͷ��㣬ͷ�������������
		try
		{
			auto pt = this->searchp(seqnum - 1);//����ѡ���õ�ǰָ��ptָ��Ҫɾ��������һ��λ�ã�����Ϊ�˱����޸�Ҫɾ��������һ������next
			auto pd = pt->next;//����Ϊ���ڸ�����������֮���ͷ���ɾ�������ڴ�
			pt->next = pt->next->next;
			delete pd;
			--this->length;
			return true;
		}
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{//����catch����ֻ��һ����䣬Ҳ����ʹ�ô�����
			return false;
		}
	}

	virtual WangpaiList& merge(WangpaiList& other)//�����˸ú���֮������other�Ͳ�������
	{
		auto pt = searchp(this->length);
		pt->next = other.head->next;
		this->length += other.length;
		other.length = 0;
		other.head->next = nullptr;
		return *this;
	}

	virtual void traverse(void(*pfun)()) = delete;//pfun��point function ����ָ��

	friend ostream& operator<<(ostream& os, WangpaiList& mylist)//�������Ҫ��֤DataType�Ѿ�������<<��������غ���
	{
		cout << mylist.information << endl;
		int order = 1;
		auto pt = mylist.head->next;
		for (; order <= mylist.length; ++order, pt = pt->next)
		{
			cout << "��" << order << "���������ݣ�" << pt->data << endl;
		}
		return os;
	}

#undef defaultname
//����Ĵ��벻��Ҫ�˺꣬��������ȥ��ǰ��ĺ궨��
#endif
};

