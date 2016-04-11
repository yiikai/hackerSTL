#ifndef HASH_FUN_H
#define HASH_FUN_H

template<typename T>
struct hackhash{};

size_t __stl_hash_string(const char* __s)
{
	unsigned long __h = 0;
	for (; *__s; ++__s)
		__h = 5 * __h + *__s;

	return size_t(__h);
}


template<>
struct hackhash < char* > {
	size_t operator()(const char* data)
	{
		return __stl_hash_string(data);
	}
};

template<>
struct hackhash <const char* > {
	size_t operator()(const char* data)
	{
		return __stl_hash_string(data);
	}
};

template<>
struct hackhash < int > {
	size_t operator()(int data)
	{
		return data;
	}
};

template<>
struct hackhash < char > {
	size_t operator()(char data)
	{
		return data;
	}
};


#endif