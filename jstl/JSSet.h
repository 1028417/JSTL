
#ifndef __JSSet_H
#define __JSSet_H

#include "Container.h"

#include <unordered_set>
#include <set>

namespace NS_JSTL
{
	template<typename __DataType, template<typename...> class __SetType=set>
	class JSSet : public ContainerT<__DataType, __SetType<__DataType>>
	{
	private:
		using __SuperClass = ContainerT <__DataType, __SetType<__DataType>>;

		using __JSSet_InitList = __InitList<__DataType>;

		using __JSSet_CB = __FN_CB<__DataType>;
		using __JSSet_CB_RetBool = __FN_CB_RetBool<__DataType>;

		using __ConstDataRef = const __DataType&;

	public:
		JSSet()
		{
		}

		template<typename... args>
		explicit JSSet(__ConstDataRef data, const args&... others)
		{
			__SuperClass::add(data, others...);
		}

		explicit JSSet(const JSSet& set)
			: __SuperClass(set)
		{
		}

		JSSet(JSSet&& set)
		{
			__SuperClass::swap(set);
		}

		explicit JSSet(__JSSet_InitList initList)
			: __SuperClass(initList)
		{
		}

		template<typename T, typename _ITR = decltype(declval<T>().begin())>
		explicit JSSet(const T& container)
			: __SuperClass(container)
		{
		}

		JSSet& operator=(const JSSet& set)
		{
			__SuperClass::assign(set);
			return *this;
		}

		JSSet& operator=(JSSet&& set)
		{
			__SuperClass::swap(set);
			return *this;
		}

		JSSet& operator=(__JSSet_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		JSSet& operator=(const T&t)
		{
			__SuperClass::assign(t);
			return *this;
		}

	private:
		TD_SizeType _add(__ConstDataRef data) override
		{
			__SuperClass::m_data.insert(data);

			return __SuperClass::m_data.size();
		}

		TD_SizeType _del(__ConstDataRef data) override
		{
			auto itr = __SuperClass::m_data.find(data);
			if (itr == __SuperClass::m_data.end())
			{
				return 0;
			}

			__SuperClass::m_data.erase(itr);

			return 1;
		}

		bool _includes(__ConstDataRef data) const override
		{
			return __SuperClass::m_data.find(data) != __SuperClass::m_data.end();
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

		template <typename T>
		JSSet<T, __SetType> map(__FN_CB<__DataType, T> fn) const
		{
			JSSet<T, __SetType> ret;

			for (auto&data : __SuperClass::m_data)
			{
				ret.add(fn(data));
			}

			return ret;
		}

		template <typename __Function>
		auto map(__Function fn) const ->JSSet<decltype(fn(__DataType())), __SetType> const
		{
			JSSet<decltype(fn(__DataType())), __SetType> ret;
			
			for (auto&data : __SuperClass::m_data)
			{
				ret.add(fn(data));
			}

			return ret;
		}

		JSSet filter(__JSSet_CB_RetBool fn) const
		{
			JSSet set;

			for (auto&data : __SuperClass::m_data)
			{
				if (fn(data))
				{
					set.add(data);
				}
			}

			return set;
		}
	};

	template <typename __DataType>
	using JSUnsortSet = JSSet<__DataType, unordered_set>;
}

#endif // __JSSet_H
