#ifndef HACKER_PRIORITY_QUEUE_H
#define HACKER_PRIORITY_QUEUE_H
#include "hackerHeap.h"
template < typename T, typename compare, typename sequence = hackervector<T> >
class hackerPriorityQueue
{
public:
	explicit hackerPriorityQueue(sequence vec) :c(vec)
	{
		heap_make(c.begin(), c.end(),c.size(), m_cmp);
		hackervector<T>::iterator itr = c.begin();
		for (; itr != c.end(); itr++)
		{
			printf("%d\n", *itr);
		}
	}
	template <typename iterator>
	hackerPriorityQueue(iterator start, iterator last) : c(start, last)
	{
		heap_make(c.begin(), c.end(),c.size(), m_cmp);
	}

	T top()
	{
		return c.front();
	}

	bool empty()
	{
		return c.empty();
	}

	size_t size()
	{
		return c.size();
	}

	void pop()
	{
		heap_pop(c.begin(), c.end(),c.size(), m_cmp);
		c.pop_back();
	}

	void push(const T& value)
	{
		c.push_back(value);
		push_heap(c.begin(),c.end(),c.size(),m_cmp);
	}
	~hackerPriorityQueue(){}
private:
	sequence c;
	compare m_cmp;
	
};

#endif