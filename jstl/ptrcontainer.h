
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

	ptrcontainerT(__Super&& container)
	{
		swap(container);
	}

	ptrcontainerT(const ptrcontainerT& container)
		: __Super(container.begin(), container.end())
	{
	}

	explicit ptrcontainerT(const vector<__PtrType>& container)
		: __Super(container.begin(), container.end())
	{
	}

	explicit ptrcontainerT(const list<__PtrType>& container)
		: __Super(container.begin(), container.end())
	{
	}

	template<class _Iter, typename = checkIter_t<_Iter>>
	explicit ptrcontainerT(_Iter _First, _Iter _Last)
		: __Super(_First, _Last)
	{
	}

	explicit ptrcontainerT(__PtrType ptr)
	{
		_add(ptr);
	}

	explicit ptrcontainerT(__RefType ref)
	{
		_add(ref);
	}

	template <typename T>
	explicit ptrcontainerT(const vector<T>& container)
	{
		_addContainer(container);
	}

	template <typename T>
	explicit ptrcontainerT(list<T>& container)
	{
		_addContainer(container);
	}

	template <typename T>
	explicit ptrcontainerT(const list<T>& container)
	{
		_addContainer(container);
	}

private:
	template <typename T>
	void _addContainer(T& container)
	{
		for (auto& ptr : container)
		{
			_add(ptr);
		}
	}

	void _add(__PtrType ptr)
	{
		if (NULL != ptr)
		{
			push_back(ptr);
		}
	}
	
	void _add(__RefType ref)
	{
		push_back(&ref);
	}

	template <typename T>
	void _add(T* ptr, ...)
	{
		_add(dynamic_cast<__PtrType>(ptr));
	}

	template <typename T>
	void _add(T& ref, ...)
	{
		_add(dynamic_cast<__PtrType>(&ref));
	}

public:
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

	ptrcontainerT& clear()
	{
		__Super::clear();
		return *this;
	}

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

	template <typename T>
	ptrcontainerT& add(const vector<T>& container)
	{
		_addContainer(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(vector<T>& container)
	{
		_addContainer(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(const list<T>& container)
	{
		_addContainer(container);
		return *this;
	}

	template <typename T>
	ptrcontainerT& add(list<T>& container)
	{
		_addContainer(container);
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
