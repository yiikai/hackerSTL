// MemoryHacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "hackerVector.h"
#include "hackerList.h"
#include "hackerHeap.h"
#include "hackerPriorityQueue.h"
#include "hackerRBTree.h"
#include "hackerString.h"
#include "hackerhashtable.h"
#include "hashfun.h"
using namespace std;
class getint
{
public:
	int operator()(int x)
	{
		return x;
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	hackerHashTable<int, hackhash<int>, getint> hasher;
	hasher.insert_unequle(1);
	return 0;
}

