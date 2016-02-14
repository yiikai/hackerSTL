#include <new>
template<typename T>
void construct(void* p , T n)
{
	new((void*)p) T(n);
}

template<typename T>
void destroy(T p)
{
	p.~T();
}

template<typename T>
void destroy(T* p)
{
	p->~T();
}
struct false_type
{

};

struct true_type
{

};
template < typename T >
void destroy(T* p, true_type)
{
	printf("not destroy do nothing\n");
}

template <typename iterator>
void destroy(iterator begin, iterator end, true_type)
{
	
}

template <typename iterator>
void destroy(iterator begin, iterator end, false_type)
{
	for (; begin != end; begin++)
		destroy(begin);
}

template < typename T >
void destroy(T* p, false_type)
{
	p->~T();
}


template<typename T>
struct type_traits
{
	typedef struct false_type has_trivial_default_destroy;
};

template<>
struct type_traits<int>
{
	typedef struct true_type has_trivial_default_destroy;
};

//迭代器类型萃取器
template<typename iterator>
struct iterator_traits
{
	typedef  typename iterator::value_type value_type;
};

template<typename T>
struct iterator_traits < T* >
{
	typedef T value_type;
};
