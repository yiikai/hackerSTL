
#define MAXALLOCMEMSIZE 128
#define MEMPOOLSIZE 2024

#include <stdlib.h>
template <typename T>
class MemHacker
{
	typedef T* POINTER;
public:
	MemHacker(){
		m_startpool = m_endpool = mempool;
		m_endstorgaepool = m_startpool + MAXALLOCMEMSIZE;
	}
	~MemHacker(){}

	static char mempool[MEMPOOLSIZE];
	static void* HackerAllocate(int n);
	static void HackerDestroy(POINTER p);
	//static char* HackerDeallocate(void *p ,unsigned int n);
private:
	static char* m_startpool;
	static char* m_endpool;
	static char* m_endstorgaepool;
};



template <typename T>
char MemHacker<T>::mempool[MEMPOOLSIZE] = { 0 };
template <typename T>
char* MemHacker<T>::m_startpool = mempool;
template <typename T>
char* MemHacker<T>::m_endpool = mempool;
template <typename T>
char* MemHacker<T>::m_endstorgaepool = m_startpool + MAXALLOCMEMSIZE;

template <typename T>
void* MemHacker<T>::HackerAllocate(int n)
{
	char* ret = NULL;

	if (sizeof(T)*n > MAXALLOCMEMSIZE)
	{
		ret = (char*)malloc(sizeof(T)*n);
		if (!ret)
			return NULL;
	}
	else
	{
		if ((int)(sizeof(T)*n) > (int)(m_endstorgaepool - m_endpool))
		{
			ret = (char*)malloc(sizeof(T)*n);
			if (!ret)
				return NULL;
		}
		else
		{
			ret = m_endpool;
			m_endpool += sizeof(T)*n;
		}
	}
	return ret;
}

template<typename T>
void MemHacker<T>::HackerDestroy(POINTER p)
{
	destroy(p,type_traits<T>::has_trivial_default_destroy);
}