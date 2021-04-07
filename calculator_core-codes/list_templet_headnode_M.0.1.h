
//��Ϊ��ͷ��������ͷ�ļ�

#pragma once

#include <cstring>
#include <climits>
#include <iostream>
using namespace std;

template <typename DataType>
class WangpaiList
{
protected:

	class NodeInfo //NodeInfo��node information �����Ϣ
	{
	private:
		char* listname;
	public:
		NodeInfo(const char* forname = nullptr)//forname��formal parameter name �β�name
		{
			if (nullptr == this->listname)
				forname = (char*)WangpaiList::defaultname;
			this->listname = new char[strlen(forname) + 1];
			strcpy_s(this->listname, strlen(forname) + 1, forname);
		}
		NodeInfo(const NodeInfo& other)
		{
			this->listname = nullptr;
			*this = other;
		}
		~NodeInfo()
		{
			delete[]this->listname;
		}

		NodeInfo& operator=(const NodeInfo& other)
		{
			if (&other == this)
				return *this;
			delete[]this->listname;
			this->listname = new char[strlen(other.listname) + 1];
			strcpy_s(this->listname, strlen(other.listname) + 1, other.listname);
			return *this;
		}

		friend ostream& operator<<(ostream& os, NodeInfo& info)//info��information ��Ϣ
		{
			cout << "������Ϣ��" << info.listname;
			return os;
		}
	};

	template <typename DataType_>
	class Node
	{
	public:
		DataType_ data;
		Node* next;
	};

	/*-----------------���ݳ�Ա---------------------------*/
protected:

	static const char* const defaultname;
	const int MAX_LENGTH;

	NodeInfo information;
	Node<DataType>* const head;//һ����ʼ����Ͳ��ܱ��޸ģ���ͷ���ĵ�ַ��������������������ж�����仯
	int length;

	//int seqnum;//seqnum��sequence number ��� 
	//Node<DataType>* pointer;
	/*-----------------���ݳ�Ա---------------------------*/

protected:

	virtual WangpaiList& operator+(const DataType& data)
	{
		auto pt = this->findlast();
		pt->next = new Node<DataType>;
		pt->next->data = data;
		pt->next->next = nullptr;
		++this->length;
		return *this;
	}

	virtual WangpaiList& operator+(Node<DataType>* piece)//�˺���������piece��ָ����������뱣֤pieceָ��������Ƕ�����
	{
		auto pt_this = this->findlast();
		auto pt_piece = piece;
		int piece_length = 0;

		while (pt_piece)//������������Ϊ��ͳ��piece����ĳ��ȣ�pt_pieceÿָ��һ��ʵ�ʵĽ�㣬�ͼ�һ����
		{
			++piece_length;
			pt_piece = pt_piece->next;
		}//ע��ÿһ��ѭ��������piece_length����pt_piece��ָ������ţ�����ʵ��pt_piece��ָ����һ���������

		pt_this->next = piece;

		this->length += piece_length;
		piece = nullptr;
		return *this;
	}

	virtual WangpaiList& operator+(const WangpaiList& other)//ע��˺������뱣֤other�������ݵ�������
	{
		auto pt_this = this->findlast();
		auto pt_other = other.head->next;//ָ��other�����һ����Ҫ����������

		while (pt_other)//��ָ�벻Ϊ�գ�˵����ǰָ��pt_otherָ����һ��ʵ�ʵ����ݣ���ô��ִ������Ŀ���
		{
			pt_this->next = new Node<DataType>;
			pt_this->next->data = pt_other->data;
			pt_this = pt_this->next;
			pt_other = pt_other->next;
		}
		pt_this->next = nullptr;
		this->length += other.length;
		return *this;
	}

	virtual WangpaiList& operator+=(Node<DataType>* piece)//�˺���������piece��ָ����������뱣֤pieceָ��������Ƕ�����
	{
		return *this + piece;
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
				;
			return pt;
		}
	}

	virtual DataType& searchd(int seqnum)//searchd��search data �������ݵ�����
	{
		try//����try��catch����ֻ��һ����䣬Ҳ����ʹ�ô�����
		{
			return this->searchp(seqnum)->data;

		}
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{
			throw "���Ժ��� virtual DataType& WangpaiList<DataType>::search(int seqnum) �ķ���:\n"
				"   ���ṩ�����seqnum������\n\n";
		}
	}

	virtual Node<DataType>* findlast()//findlast��find the last �ҵ����Ľ��
	{
		auto pt = this->head;
		while (pt->next)//�ҵ�*this��������һ����㣺���Ľ�㲻����˵����ǰָ��ptָ��������һ�����
			pt = pt->next;
		return pt;
	}

	//bool ishead(Node<DataType>* pointer);

public:

	WangpaiList(const char* listname = nullptr, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(listname), head(new Node<DataType>), length(0)
	{
		this->head->next = nullptr;
	}

	WangpaiList(const DataType& data, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), head(new Node<DataType>), length(0)
	{
		this->head->next = nullptr;
		*this = data;
	}

	WangpaiList(const WangpaiList& other, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(other.information), head(new Node<DataType>), length(0)
	{
		this->head->next = nullptr;
		*this = other;
	}

	virtual ~WangpaiList()//һ�������˸ú�����ͷ��㶼�ᱻɾ��,����һ��������󣬴˺���ֻ��������һ�Σ���˲������ֶ�����
	{

		/*//����һ��Ŀǰʱ�临�Ӷ����ŵ��㷨��ΪO(n)��,���Ҵ��㷨��ʵ����ͷ���������޹أ�
		//���ǽϳ�����Ϊû���õ���Ա����
		auto pt = this->head;
		auto pd;
		while (pt)//��ָ�벻Ϊ�գ�˵��ָ����һ����̬�ڴ���
		{
		pd = pt;//����Ϊ���ڵ�ǰָ��ptָ����һ�����֮�󣬱��浱ǰ���ĵ�ַ�Ա��Ժ�ɾ��
		pt = pt->next;
		delete pd;
		}
		*/
		emplist();
		delete this->head;
	}

	virtual WangpaiList& operator=(const DataType& data)
	{
		emplist();
		return *this += data;
	}

	virtual WangpaiList& operator=(const WangpaiList& other)
	{
		emplist();
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
			return this->searchd(seqnum);

		}
		catch (char* erinfo)//erinfo��error information ������Ϣ
		{
			throw "���Ժ��� virtual DataType& WangpaiList<DataType>::operator[](int seqnum) �ķ���:\n"
				"   ���ṩ�����seqnum������\n\n";
		}
	}


	virtual WangpaiList& emplist()//emplist��empty the list	�������
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

	virtual int search(const DataType& data)//����涨Ԫ�ص���ž��ǵ�ɾ���������Ԫ��ʱ����ĳ���                               
	{//�˺�����ʵ����ҪDataType�Ѿ�������!=��������غ���
		auto pt = this->head->next;
		int order = 1;//��Ϊͷ��㲻�������ݣ�����Ҫ����Ԫ��㿪ʼ������order�����ָ�룬pt��ʵ��ָ�룬�����������ָ���ֵ����������
		for (;
			pt && data != pt->data;//pt��Ϊ�յ���ѭ������ʱ��˵����ǰָ��ptָ�������ݲ�����Ҫ����
			pt = pt->next, ++order)
			continue;

		if (pt)//pt��Ϊ��˵������û�б������������һ��Ԫ�ص���һ����λ�ã����ҵ�����Ҫ����
			return order;
		else
			return -1;
	}


	virtual bool inserdata(const DataType& data, int seqnum)//inserdata��insert data ��������
	{//���ǽ����ݲ����seqnum����㣬����������ݱ���˵�seqnum�������������ݣ�ԭ����Ӧλ�õĽ����ơ�
		try
		{
			auto pt = this->searchp(seqnum - 1);//����ѡ���õ�ǰָ��ptָ��Ҫ����λ�õ���һ��λ�ã�����Ϊ�˱����޸���һ������next
			auto pd = pt->next;//����Ϊ���ں����������������������������Ա����ڰ�����ֳ��������֣�����ʹ�üӷ�����
			pt->next = nullptr;//�Ͽ�����˴����벿�������ӡ�
			this->length = seqnum - 1;//�Ͽ������벿��ʱ��ͬʱҪ�޸�������
			(*this += data) += pd;//��ʼƴ��
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
		auto pt = this->findlast();
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
};

template<typename DataType>
const char* const WangpaiList<DataType>::defaultname = "����������ͷ���";

