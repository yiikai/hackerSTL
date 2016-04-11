#ifndef HACKER_HASH_TABLE_H
#define HACKER_HASH_TABLE_H

#include "memoryhacker.h"
#include "hackerVector.h"
#include <iostream>
using namespace std; 
#define __stl_num_primes 28

static const unsigned long __stl_prime_list[__stl_num_primes] =
{
	53ul, 97ul, 193ul, 389ul, 769ul,
	1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
	1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
	50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};

template < typename var>
class simpleHashNodeAlloc
{
	typedef var* var_point;
	typedef simple_alloc<var> node_alloc;
public:
	simpleHashNodeAlloc(){}
	~simpleHashNodeAlloc(){}
	static var_point createhashnode()
	{
		return (var_point)node_alloc::simple_malloc(sizeof(var));
	}
};


template <typename var, typename hashfunc, typename _ExtractKey, typename Alloc = simpleHashNodeAlloc<var>>
class hackerHashTable
{
	
private:
	typedef var value_type;
	typedef var* value_type_pointer;
	typedef _ExtractKey get_key;
	typedef hashfunc hasher;
	typedef struct varnode
	{
		value_type member;
		varnode* next;
	}VAR_NODE;
	typedef simpleHashNodeAlloc<VAR_NODE> simple_hashNode_alloc;
public:
	hackerHashTable() :buccketnum(0), m_currenttablessize(0){}
	~hackerHashTable(){}

	void insert_unequle(value_type obj);
private:
	void insert_equal_noresize(value_type& obj);
private:
	unsigned long GetTableSize(unsigned long num);
	void tableResize(unsigned long num);
private:
	hackervector<VAR_NODE*> m_table;
	hasher m_hashfunc;
	get_key m_getkeyfunc;
	unsigned long buccketnum;
	int m_currenttablessize;
};

template <typename var, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
unsigned long hackerHashTable<var, hashfunc, _ExtractKey,Alloc>::GetTableSize(unsigned long num)
{
	int i = 0;
	for (; i < __stl_num_primes && __stl_prime_list[i] < num; i++)
	{

	}
	if (i == __stl_num_primes)
	{
		return __stl_num_primes - 1;
	}
	return i;
}


template <typename var, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
void hackerHashTable<var, hashfunc, _ExtractKey, Alloc>::insert_unequle(value_type obj)
{
	tableResize(++m_currenttablessize);
	//将值插入table表中
	insert_equal_noresize(obj);
}

template <typename var, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
void  hackerHashTable<var, hashfunc, _ExtractKey, Alloc>::insert_equal_noresize(value_type& obj)
{
	size_t key = m_getkeyfunc(obj);
	size_t hashvalue = m_hashfunc(key) % m_currenttablessize;
	VAR_NODE* __first = m_table[hashvalue];
	for (VAR_NODE* cur = __first; cur;cur->next)
	{
		if (m_getkeyfunc(cur->member) == key)
		{
			cout << "the key is exist" << endl;
			return;
		}
	}
	VAR_NODE* _tmp = simple_hashNode_alloc::createhashnode();
	_tmp->member = obj;
	_tmp->next = NULL;
	m_table[hashvalue] = _tmp;
}

template <typename var, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
void hackerHashTable<var, hashfunc, _ExtractKey, Alloc>::tableResize(unsigned long num)
{
	unsigned long tablesize = 0;
	if (m_currenttablessize == 0)
	{
		buccketnum = 1;   //如果这个时候没有创建table则大小用第一个质数的大小
		tablesize = __stl_prime_list[buccketnum];
	}
	else
	{
		for (int i = 0; i < __stl_num_primes; i++)
		{
			if (num <= __stl_prime_list[i])
			{
				buccketnum = i;
				tablesize = __stl_prime_list[i];
				break;
			}
		}
	}
	if (m_currenttablessize != tablesize)
	{
		//重新调整vector的大小
		m_table.resize(tablesize);
		m_currenttablessize = tablesize;
	}
}
#endif