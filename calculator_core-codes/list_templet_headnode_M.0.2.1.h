
//此为带头结点的链表头文件

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

	class NodeInfo //NodeInfo：node information 结点信息
	{//注意，内部类的成员不属于外部类，如果要使用内部类的成员，请直接定义内部类对象
	private:
#ifndef defaultname
/*这里使用了宏，这意味着前面不能使用同名的宏。
如果使用了同名的宏，下面的一部分代码将被去掉，这将导致编译错误。这可以很好的提醒用户*/
#define defaultname "此为链表的结点信息"

		string listname;

	public:

		//此方法还充当默认构造函数。注意，默认构造函数只能有一个
		//forname：formal parameter name 形参name
		explicit NodeInfo(const string forname = defaultname) :listname(forname){}
		explicit NodeInfo(const char* const forname) :listname(string(forname)){}


		friend ostream& operator<<(ostream& os, NodeInfo& info)//info：information 信息
		{
			cout << "链表信息：" << info.listname;
			return os;
		}

#undef defaultname
//后面的代码不需要此宏，所以这里去掉前面的宏定义
#endif
	};

	template <typename DataType_>
	class Node
	{
	public:
		DataType_ data;
		Node* next;

	public:

		//此方法还充当默认构造函数。注意，默认构造函数只能有一个
		explicit Node(Node* const pnt = nullptr) :next(pnt){}
		explicit Node(const DataType_& rdata, Node* const pnt = nullptr) :data(rdata), next(pnt){}
		explicit Node(const Node& other) = delete;//结点不允许直接复制

		Node& operator=(const Node& other) = delete;//结点不允许直接复制
	};

	/*-----------------数据成员---------------------------*/
protected:

#ifndef defaultname
	/*这里使用了宏，这意味着前面不能使用同名的宏。
	如果使用了同名的宏，下面的一部分代码将被去掉，这将导致编译错误。这可以很好的提醒用户*/
#define defaultname "此链表设有头结点"

	const int MAX_LENGTH;

	NodeInfo information;
	Node<DataType>* const head;//一旦初始化后就不能被修改，即头结点的地址在链表的整个生命周期中都不会变化
	int length;//链表的长度为链表元素的个数，此个数不计头结点。

	/*-----------------数据成员---------------------------*/

protected:

	virtual WangpaiList& operator+(const DataType& data)
	{
		inserdata(data, this->length + 1);
		return *this;
	}

	virtual WangpaiList& operator+(const WangpaiList& other)//注意此函数必须保证other链表数据的完整性
	{
		auto pt_this = searchp(this->length);
		auto pt_other = other.head->next;//指向other链表第一个需要拷贝的数据

		while (pt_other)//此指针不为空，说明当前指针pt_other指向了一个实际的数据，那么就执行下面的拷贝
		{
			pt_this->next = new Node<DataType>(pt_other->data);
			pt_this = pt_this->next;
			pt_other = pt_other->next;
		}
		pt_this->next = nullptr;
		this->length += other.length;
		return *this;
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
				continue;
			return pt;
		}
	}

public:

	//此方法还充当默认构造函数。注意，默认构造函数只能有一个
	explicit WangpaiList(const string listname = defaultname, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(listname), head(new Node<DataType>), length(0){}

	explicit WangpaiList(const char* const listname, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(string(listname)), head(new Node<DataType>), length(0){}

	explicit WangpaiList(const DataType& data, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(defaultname), head(new Node<DataType>), length(0)
	{
		*this = data;//后面的工作将由 =运算符重载函数 来完成
	}

	explicit WangpaiList(const WangpaiList& other, int max_length = INT_MAX)
		:MAX_LENGTH(max_length), information(other.information), head(new Node<DataType>), length(0)
	{
		*this = other;//后面的工作将由 =运算符重载函数 来完成
	}

	virtual ~WangpaiList()//一旦调用了该函数，头结点都会被删除,对于一个链表对象，此函数只允许被调用一次，因此不允许手动调用
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
		try//就算try、catch后面只有一条语句，也必须使用大括号
		{
			return searchp(seqnum)->data;

		}
		catch (char* erinfo)//erinfo：error information 错误信息
		{
			throw "来自函数 virtual DataType& WangpaiList<DataType>::operator[](int seqnum) 的反馈:\n"
				"   你提供的序号seqnum有问题\n\n";
		}
	}

	virtual WangpaiList& deleteall()//deleteall：delete all	删除所有
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

	//寻找data，返回首次找到的序号
	virtual int search(const DataType& data)                               
	{//此函数的实现需要DataType已经定义了==运算符重载函数
	 //此函数将从序号为1的元素开始搜索（也就是说，不搜索头结点，因为头结点不存有数据）
	 //这里规定元素的序号就是当删除其后所有元素时链表的长度。比如：头结点的序号为0

		/*//这是上一个版本的定义，一种对其他方法依赖性相对弱的版本
		auto& myself = *this;
		for (int order = 1; order <= this->length; ++order)//每次循环结束，order指向的是下一个比较对象的序号
		if (data == myself[order])
			return order;
		return -1;
		*/

		return this->search(data, 1, this->length);
	}

	//在begin-end之间寻找data，返回首次找到的序号
	virtual int search(const DataType& data, int begin, int end)
	{//此函数的实现需要DataType已经定义了==运算符重载函数
	 //这里规定元素的序号就是当删除其后所有元素时链表的长度。比如：头结点的序号为0

	 /*之所以额外定义这个重载的search函数，而不将之前的一个参数的search函数增加两个额外默认参数，
	 是因为“将成员作为默认参数使用要求静态成员”，因此end不能使用this->length作为默认参数*/

		if (begin < 1 || end > this->length)
			return -1;

		auto& myself = *this;
		for (int order = begin; order <= end; ++order)//每次循环结束，order指向的是下一个比较对象的序号
		if (data == myself[order])
			return order;

		return -1;
	}

	virtual int locate(const Node<DataType>* pt)
	{
		for (int order = 0; order <= this->length;++order)//每次循环结束，order指向的是下一个比较对象的序号
		if (pt == searchp(order))
				return order;
		return -1;
	}


	virtual bool inserdata(const DataType& data, int seqnum)//inserdata：insert data 插入数据
	{//这是将数据插入第seqnum个结点，即插入的数据变成了第seqnum个结点的数据内容，原来相应位置的结点后移。
		try
		{
			auto pt = this->searchp(seqnum - 1);//这里选择让当前指针pt指向要插入位置的上一个位置，这是为了便于修改上一个结点的next
			pt->next = new Node<DataType>(data, pt->next);
			++this->length;
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
		auto pt = searchp(this->length);
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

#undef defaultname
//后面的代码不需要此宏，所以这里去掉前面的宏定义
#endif
};

