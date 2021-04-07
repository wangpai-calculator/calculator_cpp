
//此为带头结点的链表头文件

#pragma once

#include <cstring>
#include <climits>
#include <iostream>
using namespace std;

template <typename DataType>
class WangpaiList
{
protected:

	class NodeInfo //NodeInfo：node information 结点信息
	{
	private:
		char* listname;
	public:
		NodeInfo(const char* forname = nullptr)//forname：formal parameter name 形参name
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

		friend ostream& operator<<(ostream& os, NodeInfo& info)//info：information 信息
		{
			cout << "链表信息：" << info.listname;
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

	/*-----------------数据成员---------------------------*/
protected:

	static const char* const defaultname;
	const int MAX_LENGTH;

	NodeInfo information;
	Node<DataType>* const head;//一旦初始化后就不能被修改，即头结点的地址在链表的整个生命周期中都不会变化
	int length;

	//int seqnum;//seqnum：sequence number 序号 
	//Node<DataType>* pointer;
	/*-----------------数据成员---------------------------*/

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

	virtual WangpaiList& operator+(Node<DataType>* piece)//此函数会吸收piece所指向的链表，必须保证piece指向的链表是独立的
	{
		auto pt_this = this->findlast();
		auto pt_piece = piece;
		int piece_length = 0;

		while (pt_piece)//遍历链表，这是为了统计piece链表的长度，pt_piece每指向一个实际的结点，就记一次数
		{
			++piece_length;
			pt_piece = pt_piece->next;
		}//注意每一轮循环结束后，piece_length不是pt_piece所指结点的序号，它其实是pt_piece所指的上一个结点的序号

		pt_this->next = piece;

		this->length += piece_length;
		piece = nullptr;
		return *this;
	}

	virtual WangpaiList& operator+(const WangpaiList& other)//注意此函数必须保证other链表数据的完整性
	{
		auto pt_this = this->findlast();
		auto pt_other = other.head->next;//指向other链表第一个需要拷贝的数据

		while (pt_other)//此指针不为空，说明当前指针pt_other指向了一个实际的数据，那么就执行下面的拷贝
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

	virtual WangpaiList& operator+=(Node<DataType>* piece)//此函数会吸收piece所指向的链表，必须保证piece指向的链表是独立的
	{
		return *this + piece;
	}

	virtual Node<DataType>* searchp(int seqnum)//searchp：search pointer 返回指针的搜索
	{
		if (seqnum<0 || seqnum>this->length)
			throw "来自函数 virtual Node<DataType>* WangpaiList<DataType>::searchp(int seqnum) 的反馈:\n"
			"   你提供的序号seqnum有问题\n\n";//这里使用异常实属迫不得已
		else
		{
			auto pt = this->head;
			int order = 0;//order是序号指针，pt是实际指针，这里把这两个指针的值关联了起来
			for (;
				order != seqnum;//因为当能执行到这一步时，order一定不大于seqnum，所以当order不断自增时，一定总能等于seqnum
				pt = pt->next, ++order)
				;
			return pt;
		}
	}

	virtual DataType& searchd(int seqnum)//searchd：search data 返回数据的搜索
	{
		try//就算try、catch后面只有一条语句，也必须使用大括号
		{
			return this->searchp(seqnum)->data;

		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{
			throw "来自函数 virtual DataType& WangpaiList<DataType>::search(int seqnum) 的反馈:\n"
				"   你提供的序号seqnum有问题\n\n";
		}
	}

	virtual Node<DataType>* findlast()//findlast：find the last 找到最后的结点
	{
		auto pt = this->head;
		while (pt->next)//找到*this链表的最后一个结点：其后的结点不存在说明当前指针pt指向的是最后一个结点
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

	virtual ~WangpaiList()//一旦调用了该函数，头结点都会被删除,对于一个链表对象，此函数只允许被调用一次，因此不允许手动调用
	{

		/*//这是一种目前时间复杂度最优的算法（为O(n)）,而且此算法的实现与头结点的有无无关，
		//但是较抽象，因为没有用到成员函数
		auto pt = this->head;
		auto pd;
		while (pt)//此指针不为空，说明指向了一个动态内存结点
		{
		pd = pt;//这是为了在当前指针pt指向下一个结点之后，保存当前结点的地址以便以后删除
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
		try//就算try、catch后面只有一条语句，也必须使用大括号
		{
			return this->searchd(seqnum);

		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{
			throw "来自函数 virtual DataType& WangpaiList<DataType>::operator[](int seqnum) 的反馈:\n"
				"   你提供的序号seqnum有问题\n\n";
		}
	}


	virtual WangpaiList& emplist()//emplist：empty the list	清空链表
	{//注意调用该函数不会删除头结点，头结点与链表共存亡
		for (; this->length > 0;)//这里this->length充当序号指针，指向链表最后一个元素。
			deldata(this->length);
		return *this;
	}

	virtual bool isemp()//isemp：is empty 判断是否为空
	{
		return 0 == this->length;
	}

	virtual bool isful()//isful：is full 判断是否为满
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

	virtual int search(const DataType& data)//这里规定元素的序号就是当删除其后所有元素时链表的长度                               
	{//此函数的实现需要DataType已经定义了!=运算符重载函数
		auto pt = this->head->next;
		int order = 1;//因为头结点不保存数据，所以要从首元结点开始，其中order是序号指针，pt是实际指针，这里把这两个指针的值关联了起来
		for (;
			pt && data != pt->data;//pt不为空但当循环继续时，说明当前指针pt指向了数据不是所要数据
			pt = pt->next, ++order)
			continue;

		if (pt)//pt不为空说明链表没有遍历到链表最后一个元素的下一个空位置，即找到了所要数据
			return order;
		else
			return -1;
	}


	virtual bool inserdata(const DataType& data, int seqnum)//inserdata：insert data 插入数据
	{//这是将数据插入第seqnum个结点，即插入的数据变成了第seqnum个结点的数据内容，原来相应位置的结点后移。
		try
		{
			auto pt = this->searchp(seqnum - 1);//这里选择让当前指针pt指向要插入位置的上一个位置，这是为了便于修改上一个结点的next
			auto pd = pt->next;//这是为了在后续套用其他方法，保存后段链表，以便现在把链表分成两个部分，后续使用加法操作
			pt->next = nullptr;//断开链表此处与后半部分了连接。
			this->length = seqnum - 1;//断开链表后半部分时，同时要修改链表长度
			(*this += data) += pd;//开始拼接
			return true;
		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{//就算catch后面只有一条语句，也必须使用大括号
			return false;
		}
	}

	virtual WangpaiList& forinser(const DataType& data)//forinser：forward insertion 前插法
	{
		this->inserdata(data, 1);
		return *this;
	}

	virtual WangpaiList& backinser(const DataType& data)//backinser：backward insertion 后插法
	{
		this->inserdata(data, this->length + 1);
		return *this;
	}

	virtual bool deldata(int seqnum)//deldata：delete data 删除数据
	{//注意这个函数不能删除头结点，头结点与链表共存亡
		try
		{
			auto pt = this->searchp(seqnum - 1);//这里选择让当前指针pt指向要删除结点的上一个位置，这是为了便于修改要删除结点的上一个结点的next
			auto pd = pt->next;//这是为了在更改链表连接之后，释放已删除结点的内存
			pt->next = pt->next->next;
			delete pd;
			--this->length;
			return true;
		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{//就算catch后面只有一条语句，也必须使用大括号
			return false;
		}
	}

	virtual WangpaiList& merge(WangpaiList& other)//调用了该函数之后，链表other就不存在了
	{
		auto pt = this->findlast();
		pt->next = other.head->next;
		this->length += other.length;
		other.length = 0;
		other.head->next = nullptr;
		return *this;
	}

	virtual void traverse(void(*pfun)()) = delete;//pfun：point function 函数指针

	friend ostream& operator<<(ostream& os, WangpaiList& mylist)//这个函数要保证DataType已经定义了<<运算符重载函数
	{
		cout << mylist.information << endl;
		int order = 1;
		auto pt = mylist.head->next;
		for (; order <= mylist.length; ++order, pt = pt->next)
		{
			cout << "第" << order << "个结点的数据：" << pt->data << endl;
		}
		return os;
	}
};

template<typename DataType>
const char* const WangpaiList<DataType>::defaultname = "此链表设有头结点";

