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
template<typename var>
struct varnode
{
	var member;
	varnode* next;
};

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = simpleHashNodeAlloc<var>>
class hackerHashTable;

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = simpleHashNodeAlloc<var>>
class hashtable_iterator
{
public:
	typedef hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc> ht;
	friend ht;
	typedef hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc> iterator;
	typedef varnode<var> VAR_NODE;
	typedef var VALUE_TYPE;
	hashtable_iterator(VAR_NODE* node,ht* _tab);
	~hashtable_iterator(){}
	iterator& operator++();
	VALUE_TYPE operator*(){
		return m_cur->member;
	}
	
private:
	ht* m_ht;
	VAR_NODE* m_cur;
};

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = simpleHashNodeAlloc<var>>
hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc>::hashtable_iterator(VAR_NODE* node, ht* _tab) :m_cur(node), m_ht(_tab)
{
	;
}

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = simpleHashNodeAlloc<var>>
hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc>& hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc>::operator++()
{
	const VAR_NODE* old = m_cur;
	m_cur = m_cur->next;
	if (!m_cur)
	{
		size_t n = m_ht->_M_bkt_num_key(old->member);
		while (!m_cur && n < m_ht->bucketsize())
		{
			n++;
			m_cur = m_ht->m_table[n];
		}
	}
	return *this;
}


template <typename var,typename key, typename hashfunc, typename _ExtractKey,typename Alloc = simpleHashNodeAlloc<var>>
class hackerHashTable
{
private:
	typedef key value_key;
	typedef var value_type;
	typedef var* value_type_pointer;
	typedef _ExtractKey get_key;
	typedef hashfunc hasher;
	typedef varnode<var> VAR_NODE;
	typedef hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc> iterator;
	typedef simpleHashNodeAlloc<VAR_NODE> simple_hashNode_alloc;
public:
	hackerHashTable() :buccketnum(0), m_currenttablessize(0){}
	~hackerHashTable(){}

	iterator insert_unequle(value_type obj);
	iterator insert_equle(value_type obj);
private:
	iterator insert_equal_noresize(value_type& obj);
	iterator insert_unequal_noresize(const value_type& obj);
	size_t _M_bkt_num_key(const value_type& obj);
	
private:
	unsigned long GetTableSize(unsigned long num);
	void tableResize(unsigned long num);
	size_t bucketsize(){ return m_currenttablessize; }
private:
	hackervector<VAR_NODE*> m_table;
	hasher m_hashfunc;
	get_key m_getkeyfunc;
	unsigned long buccketnum;
	size_t m_currenttablessize;
};

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
size_t hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::_M_bkt_num_key(const value_type& obj)
{
	size_t key = m_hashfunc(obj);
	hashvalue = m_hashfunc(key) % m_currenttablessize;
	return key;
}

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
unsigned long hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::GetTableSize(unsigned long num)
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


template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc> hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::insert_unequle(value_type obj)
{
	tableResize(++buccketnum);
	return insert_unequal_noresize(obj);
}
template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc> hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::insert_equle(value_type obj)
{
	tableResize(++buccketnum);
	//将值插入table表中
	return insert_equal_noresize(obj);
}

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc> hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::insert_unequal_noresize(const value_type& obj)
{
	size_t key = m_getkeyfunc(obj);
	size_t hashvalue = 0;
	if (m_currenttablessize == 0)
	{
		hashvalue = m_hashfunc(key) % 1;
	}
	else
	{
		hashvalue = m_hashfunc(key) % m_currenttablessize;
	}
	
	VAR_NODE** cur = &(m_table[hashvalue]);
	for (; *cur; (*cur)->next)
	{
		if ((*cur)->member == obj)
		{
			cout << "the key is exist" << endl;
			buccketnum--;
			return iterator((*cur),this);  //table不允许插入不同值的时候，返回的是已有的和插入的相同的成员
		}
	}
	VAR_NODE* _tmp = simple_hashNode_alloc::createhashnode();
	_tmp->member = obj;
	_tmp->next = NULL;
	if (!(*cur))
		(*cur) = _tmp;
	else
		(*cur)->next = _tmp;
	return iterator((*cur), this);
}

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
hashtable_iterator<var, key, hashfunc, _ExtractKey, Alloc>  hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::insert_equal_noresize(value_type& obj)
{
	size_t key = m_getkeyfunc(obj);
	size_t hashvalue = 0;
	if (m_currenttablessize == 0)
	{
		hashvalue = m_hashfunc(key) % 1;
	}
	else
	{
		hashvalue = m_hashfunc(key) % m_currenttablessize;
	}
	VAR_NODE** cur = &(m_table[hashvalue]);
	for (; (*cur);(*cur)->next)
	{
		//循环获取桶节点的最后一个，为了将待插入的节点插入桶
		;
	}
	VAR_NODE* _tmp = simple_hashNode_alloc::createhashnode();
	_tmp->member = obj;
	_tmp->next = NULL;
	if (!(*cur))
		(*cur) = _tmp;
	else
		(*cur)->next = _tmp;
	return iterator(*cur,this);
}

template <typename var, typename key, typename hashfunc, typename _ExtractKey, typename Alloc = MemHacker>
void hackerHashTable<var, key, hashfunc, _ExtractKey, Alloc>::tableResize(unsigned long num)
{
	unsigned long tablesize = 0;
	if (m_currenttablessize == 0)
	{
		tablesize = __stl_prime_list[buccketnum-1];
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
		if (m_currenttablessize == 0)
		{
			//安排vector大小
			m_table.assign(tablesize, NULL);
			
		}
		else
		{
			m_table.resize(tablesize);
			
		}
		m_currenttablessize = tablesize;
	}
}
#endif