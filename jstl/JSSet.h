
#ifndef __JSSet_H
#define __JSSet_H

#include "Container.h"

#include <unordered_set>
#include <set>

namespace NS_JSTL
{
	template<typename __DataType, template<typename...> class __SetType=set>
	class JSSetT : public ContainerT<__DataType, __SetType<__DataType>>
	{
	protected:
		using __ContainerType = __SetType<__DataType>;
		using __SuperClass = ContainerT<__DataType, __ContainerType>;

		using __JSSet_InitList = __InitList<__DataType>;

		using __ConstDataRef = const __DataType&;

		using __CB_void = __CB_T_void<__ConstDataRef>;
		using __CB_bool = __CB_T_bool<__ConstDataRef>;

	protected:
		__ContainerType& _data()
		{
			return __SuperClass::m_data;
		}

		const __ContainerType& _data() const
		{
			return __SuperClass::m_data;
		}

	public:
		//JSSetT operator+ (const ContainerT &lhs) const {}

		JSSetT& operator+= (const ContainerT &lhs) {
			return *this;
		}

		friend JSSetT operator+ (const JSSetT &lhs, const JSSetT &rhs)
		{
			JSSetT set(rhs);

			return set;
		}

		JSSetT()
		{
		}

		template<typename... args>
		explicit JSSetT(__ConstDataRef data, const args&... others)
		{
			__SuperClass::add(data, others...);
		}

		explicit JSSetT(const JSSetT& set)
			: __SuperClass(set)
		{
		}

		JSSetT(JSSetT&& set)
		{
			__SuperClass::swap(set);
		}

		explicit JSSetT(__JSSet_InitList initList)
			: __SuperClass(initList)
		{
		}

		template<typename T, typename _ITR = decltype(declval<T>().begin())>
		explicit JSSetT(const T& container)
			: __SuperClass(container)
		{
		}

		JSSetT& operator=(const JSSetT& set)
		{
			__SuperClass::assign(set);
			return *this;
		}

		JSSetT& operator=(JSSetT&& set)
		{
			__SuperClass::swap(set);
			return *this;
		}

		JSSetT& operator=(__JSSet_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		JSSetT& operator=(const T&t)
		{
			__SuperClass::assign(t);
			return *this;
		}

	protected:
		TD_SizeType _add(__ConstDataRef data) override
		{
			_data().insert(data);

			return _data().size();
		}

		TD_SizeType _del(__ConstDataRef data) override
		{
			auto itr = _data().find(data);
			if (itr == _data().end())
			{
				return 0;
			}

			_data().erase(itr);

			return 1;
		}

		bool _includes(__ConstDataRef data) const override
		{
			return _data().find(data) != _data().end();
		}

	public:
		template<typename... args>
		TD_SizeType add(__ConstDataRef data, const args&... others)
		{
			return __SuperClass::add(data, others...);
		}

		template<typename T>
		TD_SizeType add(const T& container)
		{
			return __SuperClass::add(container);
		}

		TD_SizeType add(__JSSet_InitList initList)
		{
			return add<__JSSet_InitList>(initList);
		}

	public:
		template <typename T>
		JSSetT<T, __SetType> map(__CB_T_RET<__ConstDataRef, T> cb) const
		{
			JSSetT<T, __SetType> ret;

			if (cb)
			{
				for (auto&data : _data())
				{
					ret.add(cb(data));
				}
			}

			return ret;
		}

		template <typename CB, typename RET = decltype(declval<CB>()(__DataType()))>
		JSSetT<RET, __SetType> map(const CB& cb) const
		{
			return map<RET>(cb);
		}

		JSSetT filter(__CB_bool cb) const
		{
			JSSetT set;

			if (cb)
			{
				for (auto&data : _data())
				{
					if (cb(data))
					{
						set.add(data);
					}
				}
			}

			return set;
		}
	};

	template <typename __DataType>
	using JSSet = JSSetT<__DataType, set>;

	template <typename __DataType>
	using JSUnsortSet = JSSetT<__DataType, unordered_set>;
}

#endif // __JSSet_H
