// MemoryHacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hackerVector.h"
#include "hackerList.h"
#include "hackerHeap.h"
#include "hackerPriorityQueue.h"
#include "hackerRBTree.h"
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

template <typename T>
class less
{
public:
	bool operator()(T itr1, T itr2)
	{
		if (itr1 < itr2)
			return true;
		else
			return false;
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	hackerRBTree<int, int,less<int>> tree;
	tree.insert_unequal(1,10);
	tree.insert_unequal(2, 9);
	tree.insert_unequal(3, 8);
	tree.insert_unequal(4, 7);
	tree.insert_unequal(5, 6);
	tree.insert_unequal(6, 5);
	tree.insert_unequal(7, 4);
	tree.insert_unequal(8, 3);
	printf("size is %d\n", tree.size());
	hackerRBTree<int, int, less<int>>::iterator itr;
	itr = tree.findByKey(6);
	tree.remove_node(itr);
	printf("size is %d\n", tree.size());
	return 0;  
}

