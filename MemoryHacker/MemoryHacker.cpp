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
	tree.insert_equal(1,10);
	tree.insert_equal(2, 9);
	tree.insert_equal(3, 8);
	tree.insert_equal(4, 7);
	tree.insert_equal(5, 6);
	tree.insert_equal(6, 5);
	tree.insert_equal(7, 4);
	tree.insert_equal(8, 3);
	return 0;  
}

