#ifndef HACKER_VECTOR_H
#define HACKER_VECTOR_H
#include "commonalloc.h"
#include "memoryhacker.h"

template <typename iterator>
size_t distance(iterator first, iterator last)
{
	size_t n = 0;
	while (first != last)
	{
		n++;
		first++;
	}
	return n;
}

template<typename T, typename Alloc >
class base_vector
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T& reference;
	base_vector() :_mStart(nullptr), _mFinsih(nullptr), _mStorageEnd(nullptr){}
	~base_vector(){}
protected:
	iterator _mStart;
	iterator _mFinsih;
	iterator _mStorageEnd;
};

template<typename T, typename Alloc = MemHacker>
class hackervector : public base_vector < T, Alloc >
{
public:
	typedef base_vector<T, Alloc> Base;
	typedef Alloc memalloc;
	typedef T itreator_type;

public:
	hackervector() :Base(){
		_mStorageEnd = _mFinsih = _mStart;
	}
	explicit hackervector(int num) :Base()
	{
		T* p = (T*)memalloc::HackAlloc(sizeof(T)*num);
		_mStart = p;
		_mFinsih = _mStart;
		_mStorageEnd = _mStart + num;
	}
	
	hackervector(iterator first, iterator last)
	{
		size_t n = distance(first, last);
		hackervector(n*2);
		while (first != last)
		{
			push_back(*first);
			first++;
		}
	}

	hackervector(const hackervector<T>& vec)
	{
		int num = vec.size()*sizeof(T) * 2;
		T* p = (T*)memalloc::HackAlloc(num);
		_mStart = p;
		_mFinsih = _mStart;
		_mStorageEnd = _mStart + num;
		hackervector<T>::const_iterator itr = vec.begin();
		for (; itr != vec.end(); itr++)
		{
			push_back(*itr);
		}
	}

	iterator begin()
	{
		return _mStart;
	}

	const_iterator begin() const{
		return _mStart;
	}

	iterator end()
	{
		return _mFinsih;
	}

	const_iterator end() const{
		return _mFinsih;
	}

	reference operator[](int num)
	{
		return *(begin() + num);
	}

	void push_back(const T& data){
		if (_mFinsih != _mStorageEnd)
		{
			;
		}
		else
		{
			int resize = 0;
			if (size() != 0)
			{
				resize = size() * 2 * sizeof(T);
			}
			else
			{
				resize = 2 * sizeof(T);
			}
			
			T* p = (T*)memalloc::HackAlloc(resize);
			memset((void*)p, 0, resize);
			if (_mStart)
			{
				memcpy((void*)p, (void*)_mStart, resize);
				destroy(_mStart, _mFinsih, type_traits<T>::has_trivial_default_destroy());
				memalloc::HackDealloc(_mStart, size()*sizeof(T));
			}
			_mFinsih = p + size();
			_mStart = p;
			_mStorageEnd = _mStart + resize;

		}
		construct(_mFinsih, data);
		_mFinsih++;
	}


	void copy(iterator position, iterator start_position, iterator end_position)
	{
		for (; start_position != end_position; start_position++)
		{
			*position = *start_position;
			position++;
		}
	}

	T& front()
	{
		return *_mStart;
	}

	T& back()
	{
		return *(_mFinsih - 1);
	}

	void pop_back()
	{
		--_mFinsih;
		destroy(_mFinsih);
	}

	void erase(iterator itr)
	{
		destroy(itr, type_traits<T>::has_trivial_default_destroy());
		copy(itr, itr + 1, _mFinsih);
		destroy(_mFinsih, type_traits<T>::has_trivial_default_destroy());
		_mFinsih--;
	}

	int size()
	{
		return _mFinsih - _mStart;
	}

	int size() const{
		return _mFinsih - _mStart;
	}

	bool capacity()
	{
		if (_mFinsih == _mStart)
			return true;
		else
			return false;
	}

	unsigned long storage()
	{
		return _mStorageEnd - _mStart;
	}

	bool resize(int newsize)
	{
		if (newsize > size())
		{
			T* p = (T*)memalloc::HackAlloc(sizeof(T)*newsize);
			iterator tmpStart = p;
			iterator tmpFinsih = p;
			iterator tmpStorageEnd = p + sizeof(T)*newsize;
			memcpy(tmpStart,_mStart,size());
			tmpFinsih = tmpStart + size();
			memalloc::HackDealloc(_mStart, storage());
			_mStart = tmpStart;
			_mFinsih = tmpFinsih;
			_mStorageEnd = tmpStorageEnd;
			return true;
		}
		else
		{
			return false;
		}
	}

	~hackervector(){}


};

#endif