#ifndef HACKER_LIST_H
#define HACKER_LIST_H
#include "memoryhacker.h"

template < typename T >
struct NODE_ST
{
	struct NODE_ST *prev;
	struct NODE_ST *next;
	T data;
};

template<typename T>
class list_iterator
{
	typedef struct NODE_ST<T> node;
public:
	list_iterator(){}
	~list_iterator(){}
	list_iterator(node* _node)
	{
		m_node = _node;
	}

	node* operator->()
	{
		return m_node;
	}

	T& operator*()
	{
		return m_node->data;
	}

	list_iterator operator++()
	{
		m_node = m_node->next;
		return *this;
	}

	list_iterator operator--()
	{
		m_node = m_node->prev;
		return *this;
	}

	list_iterator operator++(int)
	{
		list_iterator tmp = *this;
		m_node = m_node->next;
		return tmp;
	}

	bool operator!=(const list_iterator& x)
	{
		if (x.m_node != this->m_node)
		{
			return true;
		}
		return false;
	}

	list_iterator operator--(int)
	{
		list_iterator tmp = *this;
		m_node = m_node->prev;
		return tmp;
	}
public:
	node* m_node;
};

template<typename T,typename Alloc = MemHacker>
class hackerList
{
	typedef struct NODE_ST<T> node;
	typedef simple_alloc<node> Memhacker;
public:
	typedef list_iterator<T> iterator;
public:
	hackerList() :_mNode(nullptr){
		_mNode = GetANode();
		_mNode->prev = _mNode;
		_mNode->next = _mNode;
	}
	~hackerList(){}

	void push_back(T& data);
	iterator end()
	{
		return _mNode;
	}

	iterator begin()
	{
		return _mNode->next;
	}

	T& front()
	{
		return *(begin());
	}

	iterator erase(iterator& position)
	{
		position->next->prev = position->prev;
		position->prev->next = position->next;
		node* nextnode = position.m_node->next;
		destroy(position.m_node, type_traits<T>::has_trivial_default_destroy());
		Memhacker::simple_dealloc(position.m_node,1);
		position.m_node = nullptr;
		return nextnode;
	}

	size_t size()
	{
		return distance(begin(), end());
	}
private:
	node* GetANode()
	{
		return (node*)Memhacker::simple_malloc(1);
	}
	size_t distance(iterator begin, iterator end)
	{
		int size = 0;
		while (begin != end)
		{
			size++;
			begin++;
		}
		return size;
	}

	void insert(iterator itr,T& data)
	{
		node* tmpnode = GetANode();
		tmpnode->prev = itr->prev;
		tmpnode->next = itr.m_node;
		itr->prev->next = tmpnode;
		itr->prev = tmpnode;
		construct(&(tmpnode->data), data);
	}

private:
	node* _mNode;  //这个节点地址永远代表了list的尾部end

};

template<typename T, typename Alloc>
void hackerList<T,Alloc>::push_back(T& data)
{
	insert(end(),data);
}




#endif
