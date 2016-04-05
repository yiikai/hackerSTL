#include "commonalloc.h"
#include "cow.h"
//hackerString: useing copy on write 


class hackerString : private cow
{
public:
	hackerString(const char* pStr);
	hackerString();
	~hackerString();
	hackerString(hackerString& copy);
	hackerString& operator=(hackerString& str);
	const char* getCString();
private:
	void delRef();
private:
	/*struct str
	{
	str() :size(0),pStr(NULL),useCount(0){}
	size_t size;
	char* pStr;
	size_t useCount;
	};*/
	size_t pSize;
	char* pStr;
};

hackerString::hackerString(const char* str) :cow(),pSize(0)
{
	if (pStr != NULL)
	{
		size_t size = strlen(str);
		pSize = size;
		pStr = new char[pSize];
		memset((void*)pStr, 0, pSize);
		memcpy((void*)pStr, str, pSize);
		addRef();
	}
}

hackerString::hackerString() :cow(), pSize(0), pStr(NULL)
{
}

hackerString::~hackerString()
{
	delRef();
	if (getRefCount() == 0)
	{
		delete[] pStr;
		pStr = NULL;
		pSize = 0;
	}
}

void hackerString::delRef()
{
	cow::delRef();
	if (0 == getRefCount() && pStr != NULL)
	{
		delete[] pStr;
		pStr = NULL;
	}
}

hackerString& hackerString::operator = (hackerString& str)
{
	if (this == &str)
	{
		return *this;
	}
	
	delRef();
	pStr = str.pStr;
	pSize = str.pSize;
	str.addRef();
	setREF(str.getRefCount());
	return *this;
}

hackerString::hackerString(hackerString& copy) :cow(),pStr(NULL)
{
	*this = copy;
}

const char* hackerString::getCString()
{
	return pStr;
}