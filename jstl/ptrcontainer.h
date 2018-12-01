
#ifndef __ptrcontainer_H
#define __ptrcontainer_H

#include <list>
using namespace std;

template <template<typename...> typename __BaseType, class _PtrType>
class ptrcontainerT : public __BaseType<_PtrType>
{
	using _RefType = decltype(*declval<_PtrType>());

	using __Super = __BaseType<_PtrType>;

public:
	ptrcontainerT()
	{
	}

	ptrcontainerT(_PtrType ptr)
	{
		add(ptr);
	}

	ptrcontainerT(_RefType& ref)
	{
		add(ref);
	}

#ifdef _MSC_VER
	template<class _Iter,
		class = enable_if_t<_Is_iterator<_Iter>::value>>
#else
	template<class _Iter,
		typename = std::_RequireInputIter<_Iter>>
#endif
	ptrcontainerT(_Iter _First, _Iter _Last)
		: __Super(_First, _Last)
	{
	}

	ptrcontainerT(const list<_PtrType>& container)
		: __Super(container.begin(), container.end())
	{
	}

	ptrcontainerT(const vector<_PtrType>& container)
		: __Super(container.begin(), container.end())
	{
	}

	template <typename T>
	ptrcontainerT(const list<T*>& container)
	{
		add(container);
	}

	template <typename T>
	ptrcontainerT(list<T*>& container)
	{
		add(container);
	}

	template <typename T>
	ptrcontainerT(list<T>& container)
	{
		add(container);
	}

	template <typename T>
	ptrcontainerT(const list<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
	}

	template <typename T>
	ptrcontainerT(const vector<T*>& container)
	{
		add(container);
	}

	template <typename T>
	ptrcontainerT(vector<T*>& container)
	{
		add(container);
	}

	template <typename T>
	ptrcontainerT(vector<T>& container)
	{
		add(container);
	}

	template <typename T>
	ptrcontainerT(const vector<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
	}

public:
	void add(_PtrType ptr)
	{
		push_back(ptr);
	}

	void add(_RefType& ref)
	{
		push_back(&ref);
	}

	void del(_PtrType ptr)
	{
		auto itr = std::find(__Super::begin(), __Super::end(), ptr);
		if (itr != __Super::end())
		{
			erase(itr);
		}
	}

	void del(const _RefType& ref)
	{
		auto itr = std::find(__Super::begin(), __Super::end(), &ref);
		if (itr != __Super::end())
		{
			erase(itr);
		}
	}

private:
	template <typename T>
	void _addPtr(T& container)
	{
		for (auto ptr : container)
		{
			add(ptr);
		}
	}

	template <typename T>
	void _addDowncast(T& container, bool bDynamicCastFlag)
	{
		for (auto& ptr : container)
		{
			auto newPtr = dynamic_cast<_PtrType>(ptr);
			if (NULL != newPtr)
			{
				add(newPtr);
			}
			else
			{
				if (bDynamicCastFlag)
				{
					add(newPtr);
				}
			}
		}
	}

	template <typename T>
	void _addRef(T& container)
	{
		for (auto& ref : container)
		{
			add(ref);
		}
	}

public:
	template <typename T>
	void add(const list<T*>& container)
	{
		_addPtr(container);
	}

	template <typename T>
	void add(list<T*>& container)
	{
		_addPtr(container);
	}

	template <typename T>
	void add(list<T>& container)
	{
		_addRef(container);
	}

	template <typename T>
	void add(const vector<T*>& container)
	{
		_addPtr(container);
	}

	template <typename T>
	void add(vector<T*>& container)
	{
		_addPtr(container);
	}

	template <typename T>
	void add(vector<T>& container)
	{
		_addRef(container);
	}

	template <typename T>
	void add(const list<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
	}

	template <typename T>
	void add(const vector<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
	}
};

template <class _RefType>
using ptrlist = ptrcontainerT<list, _RefType*>;

template <class _RefType>
using ptrvector = ptrcontainerT<vector, _RefType*>;

#endif // __ptrcontainer_H
