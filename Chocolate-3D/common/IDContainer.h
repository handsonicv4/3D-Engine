#pragma once
#include<unordered_map>
#include<Windows.h>
using namespace std;


template<class T>
class IDContainer {
protected:
	unordered_map<UINT, T> pool;
	int GenerateID() const;
	virtual void Release(T &element);
public:
	IDContainer();
	static const int INVALID = -1;
	bool Exist(UINT id) const;
	virtual void Delete(UINT id);
	virtual void Clear();
	virtual ~IDContainer();
};


template<class T>
int IDContainer<T>::GenerateID() const
{
	int i;
	for (i = 0; i < INT_MAX; i++)
	{
		if (!Exist(i))
			return i;
	}
	return INVALID;
}

template<class T>
bool IDContainer<T>::Exist(UINT id) const
{
	return pool.find(id) != pool.end();
}

template<class T>
void IDContainer<T>::Release(T & element)
{
	return;
}

template<class T>
IDContainer<T>::IDContainer()
{
}

template<class T>
void IDContainer<T>::Delete(UINT id)
{
	if (Exist(id))
	{
		Release(pool[id]);
		pool.erase(id);
	}
}

template<class T>
void IDContainer<T>::Clear()
{
	for (auto& e : pool)
	{
		Release(e.second);
	}
	pool.clear();
}

template<class T>
IDContainer<T>::~IDContainer()
{
	Clear();
}

template<class T>
class Singleton
{
public:
	static T& Instance() {
		static T instance;
		return instance;
	}
	Singleton(Singleton const&) = delete;
	Singleton& operator=(Singleton const&) = delete;
private:
	Singleton() {};
	~Singleton() {};
};