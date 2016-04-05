#ifndef COW_H
#define COW_H

class cow
{
private:
	struct USEREF
	{
		size_t useCount;
	};
public:
	cow();
	~cow();

	virtual void delRef();
	void addRef();
	USEREF* getRefCount(){ return m_useref; }
	void setREF(USEREF *set){ m_useref = set; }
private:
	USEREF *m_useref;
};

cow::cow() :m_useref(NULL)
{
	m_useref = new USEREF;
	m_useref->useCount = 0;
}

cow::~cow()
{

}

void cow::delRef() 
{
	if (0 == m_useref->useCount)
		return;
	m_useref->useCount--;
}

void cow::addRef()
{
	m_useref->useCount++;
}


#endif