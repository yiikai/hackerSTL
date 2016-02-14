#ifndef HACKER_HEAP_H
#define HACKER_HEAP_H
//heap 只对于数组和vector有用， 其他的结构基础计算结果都是未知

template <typename iterator,class compare >
void push_heap(iterator first, iterator last, int size, compare comp)
{
	int holeindex = size;
	iterator_traits<iterator>::value_type value = *(last - 1);
	iterator hole = first + size - 1;
	iterator parent = first + (holeindex) / 2 - 1;
	while (/**hole > *first*/comp(hole, first) && comp(hole, parent)/**hole > *parent*/) //当洞大于最顶端的时候 持续上溯到顶端
	{
		value = *parent;
		*parent = *hole;
		*hole = value;
		hole = parent;
		holeindex = hole - first;
		parent = first + (holeindex) / 2 - 1;
	}
	value = *(first + holeindex);
	*(first + holeindex) = value;
}


template<typename iterator,class compare>
typename iterator_traits<iterator>::value_type heap_pop(iterator first,iterator last,int size,compare comp)
{
	iterator_traits<iterator>::value_type ret = *first;
	//进行下溯
	if (size == 1)
	{
		return ret;
	}
	else
	{
		*first = *(last - 1);
		*(last - 1) = ret;
		int holeindex = 0;
		int secondchild = holeindex * 2 + 2;

		while (secondchild < size - 1)
		{
			if (/**(first + secondchild -1) > *(first + secondchild)*/comp(first + secondchild - 1, first + secondchild))
			{
				secondchild--;
			}
			ret = *(first + secondchild);
			*(first + secondchild) = *(first + holeindex);
			*(first + holeindex) = ret;
			holeindex = secondchild;
			secondchild = secondchild * 2 + 2;
			if (secondchild == size - 1)
			{
				continue;
			}
		}
		if (size - 1 == 2)
		{
			if (!comp(first, last - 2))
			{
				iterator_traits<iterator>::value_type ret = *first;
				*first = *(last - 2);
				*(last - 2) = ret;
			}
		}
		
	}
	return ret;
}

template<typename iterator,class compare>
void heap_sort(iterator first , iterator last,int size,compare comp)
{ 
	while (last - first > 1)
	{
		heap_pop(first, last--, size--, comp);
	}
}

template<typename iterator,class compare>
void heap_make(iterator first, iterator last, int size,compare comp)
{
	for (int i = size / 2 -1; i >= 0; i--)
		maxHeapify(first, last, i, size, comp);

}

template<typename iterator,class compare>
void maxHeapify(iterator first,iterator last,int i,int size,compare comp)
{
	int left = i * 2 + 1;
	int right = i * 2 + 2;
	iterator_traits<iterator>::value_type largest = i;
	while (true)
	{
		if (left < size && comp(first + left,first+i)/**(first + left) > *(first+i)*/)
			largest = left;
		if (right < size && comp(first + right,first+largest)/**(first + right) > *(first + largest)*/)
			largest = right;
		if (i != largest)
		{
			iterator_traits<iterator>::value_type data = *(first + i);
			*(first + i) = *(first + largest);
			*(first + largest) = data;
		}
		else
			break;
		i = largest;
		left = largest*2+1;
		right = largest*2+2;
	}
}

#endif