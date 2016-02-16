//定长内存池
#ifndef MEMORY_HACKER_H
#define MEMORY_HACKER_H

#include <stdlib.h>
#include <string.h>
#define MAXCHUNKSIZE 128
#define FREELISTNUM 16

union _Obj {
	union _Obj* _M_free_list_link;
	char _M_client_data[1];    /* The client sees this.        */
};

class MemHacker
{	
public:
	MemHacker()
	{

	}

	~MemHacker()
	{

	}

	static void HackDealloc(void* p,size_t n)
	{
		if (n > MAXCHUNKSIZE)
		{
			free(p);
		}
		else
		{
			size_t index = _S_freelist_index(n);
			_Obj** __my_free_list = &(_S_free_list[index]);
			if (__my_free_list)
			{
				_Obj* current = *__my_free_list;
				((_Obj*)p)->_M_free_list_link = current;
				*__my_free_list = ((_Obj*)p);
				memset(p,0,n);
			}
		}
	}


	static void* HackAlloc(size_t n)
	{
		void* result = nullptr;
		if (n > MAXCHUNKSIZE)
		{
			//use malloc 
			result = (void*)malloc(n);
		}
		else
		{
			size_t index = _S_freelist_index(n);
			_Obj** __my_free_list = &(_S_free_list[index]);
			if (*__my_free_list == nullptr)
			{
				//need alloc new buf
				result = _S_refill(_S_round_up(n));
			}
			else
			{
				//get a buf from pool and adjust pool list
				result = *__my_free_list;
				*__my_free_list = (*__my_free_list)->_M_free_list_link;

			}
			return result;
		}
	}
private:
	static void* _S_refill(size_t bytes);
	static char* _S_chunk_alloc(size_t size,size_t& nobjs);
	static size_t _S_freelist_index(size_t __bytes) {
		return (__bytes -1)/ 8;
	}
	static size_t
		_S_round_up(size_t __bytes)
	{
		return (((__bytes)+(size_t)8 - 1) & ~((size_t)8 - 1));
	}
private:
	static union _Obj* _S_free_list[FREELISTNUM];
	static size_t m_pooltotalsize;
	static char* m_freelist_start;
	static char* m_freelist_end;

};

union _Obj* MemHacker::_S_free_list[FREELISTNUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
size_t MemHacker::m_pooltotalsize = 0;
char* MemHacker::m_freelist_start = 0;
char* MemHacker::m_freelist_end = 0;

void* MemHacker::_S_refill(size_t bytes)
{
	size_t noobjs = 20;
	_Obj**  __my_free_list = nullptr;
	_Obj* __result = nullptr;
	_Obj* __current_obj = nullptr;
	_Obj* __next_obj = nullptr;
	int n = _S_freelist_index(bytes);
	__my_free_list = &(_S_free_list[n]);
	if (*__my_free_list == 0)
	{
		char* chunk = _S_chunk_alloc(bytes, noobjs);
		if (noobjs == 1)
		{
			return chunk;
		}
		__result = (_Obj*)chunk;
		*__my_free_list = (_Obj*)(chunk + bytes);
		__current_obj = __next_obj = *__my_free_list;
		for (int i = 1;; i++)
		{
			__next_obj = (_Obj*)((char*)__next_obj + bytes);
			if (noobjs - 1 == i) {
				__current_obj->_M_free_list_link = 0;
				break;
			}
			else {
				__current_obj->_M_free_list_link = __next_obj;
			}
		}
		return __result;
	}
}

char* MemHacker::_S_chunk_alloc(size_t size, size_t& nobjs)
{
	char* result = nullptr;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = m_freelist_end - m_freelist_start;
	if (bytes_left >= total_bytes)
	{
		result = m_freelist_start;
		m_freelist_start = result + total_bytes;
		return result;
	}
	else if (bytes_left >= size)
	{
		nobjs = (int)(bytes_left / size);
		result = m_freelist_start;
		m_freelist_start = result + nobjs * size;
		return result;
	}
	else
	{
		result = (char*)malloc(2 * total_bytes);  //每次请求内存的时候获取2倍的数据量
		if (!result)
		{
			printf("request memory error\n");
			return result;
		}
		m_freelist_start = result;
		m_freelist_end = m_freelist_start + 2 * total_bytes;
		m_pooltotalsize += 2 * total_bytes;

		return (_S_chunk_alloc(size, nobjs));
	}
}


template<typename T,typename Alloc = MemHacker>
class simple_alloc
{
public:
	simple_alloc(){}
	~simple_alloc(){}

	static void* simple_malloc(size_t n)
	{
		return MemHacker::HackAlloc(sizeof(T)*n);
	}

	static void simple_dealloc(void* p,size_t n)
	{
		MemHacker::HackDealloc(p, sizeof(T)*n);
	}

};


#endif