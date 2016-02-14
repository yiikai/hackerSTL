// MemoryHacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hackerVector.h"
#include "hackerList.h"
#include "hackerHeap.h"
#include "hackerPriorityQueue.h"
template <typename iterator>
class compless
{
public:
	bool operator()(iterator itr1, iterator itr2)
	{
		if (*itr1 < *itr2)
			return true;
		else
			return false;
	}
}; 

template <typename iterator>
class compbig
{
public:
	bool operator()(iterator itr1, iterator itr2)
	{
		if (*itr1 > *itr2)
			return true;
		else
			return false;
	}
}; 

int _tmain(int argc, _TCHAR* argv[])
{
	hackervector<int> vec(7);
	vec.push_back(68);
	vec.push_back(31);
	vec.push_back(65);
	vec.push_back(21);
	vec.push_back(24);
	vec.push_back(74);
	
	hackerPriorityQueue<int, compbig<hackervector<int>::iterator>> minqueue(vec);
	printf("min queue size %d\n", minqueue.size());
	printf("min data is %d\n", minqueue.top());
	minqueue.pop();
	printf("min data is %d\n", minqueue.top());
	minqueue.pop();
	printf("min data is %d\n", minqueue.top());
	minqueue.pop();
	printf("min data is %d\n", minqueue.top());
	minqueue.pop();
	printf("min data is %d\n", minqueue.top());
	minqueue.pop();
	printf("min data is %d\n", minqueue.top());
	return 0;  
}

