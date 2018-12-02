
#ifndef __ptrcontainer_H
#define __ptrcontainer_H

#include "_define.h"

#include <list>

template <template<typename...> typename __BaseType, class __PtrType>
class ptrcontainerT : public __BaseType<__PtrType>
{
private:
	using __Super = __BaseType<__PtrType>;

protected:
	using __Type = typename remove_reference<decltype(*(__PtrType)NULL)>::type;
	using __RefType = __Type&;
	using __ConstRef = const __Type&;
	using __ConstPtr = const __Type*;

public:
	ptrcontainerT()
	{
	}

	ptrcontainerT(__PtrType ptr)
	{
		add(ptr);
	}

	ptrcontainerT(__RefType ref)
	{
		add(ref);
	}

	template<class _Iter, typename = checkIter_t<_Iter>>
	ptrcontainerT(_Iter _First, _Iter _Last)
		: __Super(_First, _Last)
	{
	}

	ptrcontainerT(const list<__PtrType>& container)
		: __Super(container.begin(), container.end())
	{
	}

	ptrcontainerT(const vector<__PtrType>& container)
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
	ptrcontainerT(const list<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
	}

	template <typename T>
	ptrcontainerT(list<T>& container)
	{
		add(container);
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
	ptrcontainerT(const vector<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
	}

	template <typename T>
	ptrcontainerT(vector<T>& container)
	{
		add(container);
	}

public:
	ptrcontainerT& add(__PtrType ptr)
	{
		push_back(ptr);

		return *this;
	}

	ptrcontainerT& add(__RefType ref)
	{
		push_back(&ref);

		return *this;
	}

	bool del(__ConstPtr ptr)
	{
		auto itr = std::find(__Super::begin(), __Super::end(), ptr);
		if (itr != __Super::end())
		{
			erase(itr);
			return true;
		}

		return false;
	}

	bool del(__ConstRef ref)
	{
		return del(&ref);
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
			auto newPtr = dynamic_cast<__PtrType>(ptr);
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
	ptrcontainerT& add(const list<T*>& container)
	{
		_addPtr(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(list<T*>& container)
	{
		_addPtr(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(list<T>& container)
	{
		_addRef(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(const vector<T*>& container)
	{
		_addPtr(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(vector<T*>& container)
	{
		_addPtr(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(vector<T>& container)
	{
		_addRef(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(const list<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(const vector<T*>& container, bool bDynamicCastFlag)
	{
		_addDowncast(container, bDynamicCastFlag);
		return *this;
	}
};

template <class __PtrType>
using ptrvectorT = ptrcontainerT<vector, __PtrType>;

template <class __Type>
using ptrvector = ptrvectorT<__Type*>;

template <class __PtrType>
using ptrlistT = ptrcontainerT<list, __PtrType>;

template <class __Type>
using ptrlist = ptrlistT<__Type*>;

#endif // __ptrcontainer_H
