
#ifndef __JSSet_H
#define __JSSet_H

#include "Container.h"

#include <unordered_set>
#include <set>

namespace NS_JSTL
{
	template<typename __DataType, template<typename...> class __Container=set
		, typename __SuperClass = ContainerT<__DataType, __Container<__DataType>>
	> class JSSet : public __SuperClass
	{
	private:
		using __JSSet_InitList = __InitList<__DataType>;

		using __JSSet_CB = __FN_CB<__DataType>;
		using __JSSet_Cond = __FN_Cond<__DataType>;

	public:
		JSSet()
		{
		}

		explicit JSSet(const __DataType&v)
		{
			push(v);
		}

		explicit JSSet(const JSSet& set)
			: __SuperClass(set)
		{
		}

		JSSet(JSSet&& set)
		{
			__SuperClass::swap(set);
		}

		explicit JSSet(const __JSSet_InitList& initList)
			: __SuperClass(initList)
		{
		}

		template<typename T>
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

		JSSet& operator=(const __JSSet_InitList& initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		JSSet& operator=(const T& container)
		{
			if (__SuperClass::CheckIsSelf(container))
			{
				return *this;
			}

			__SuperClass::assign(container);
			return *this;
		}
		
	private:
		virtual TD_SizeType _add(const __DataType&v) override
		{
			__SuperClass::m_data.insert(v);

			return __SuperClass::m_data.size();
		}
		
	public:
		bool has(const __DataType&v) const override
		{
			return __SuperClass::m_data.find(v) != __SuperClass::m_data.end();
		}

		TD_SizeType add(const __DataType&v)
		{
			return _add(v);
		}

		template<typename T>
		TD_SizeType add(const T& container)
		{
			if (__SuperClass::CheckIsSelf(container))
			{
				return __SuperClass::size();
			}

			__SuperClass::m_data.insert(container.begin(), container.end());

			return __SuperClass::m_data.size();
		}

		TD_SizeType add(const __JSSet_InitList& initList)
		{
			return add<__JSSet_InitList>(initList);
		}
		
		template<typename T>
		JSSet concat(const T& container)
		{
			JSSet ret(*this);
			ret.add(container);
			return ret;
		}

		JSSet concat(const __JSSet_InitList& initList)
		{
			JSSet ret(*this);
			ret.add(initList);
			return ret;
		}

		TD_SizeType del(const __DataType&v) override
		{
			auto itr = __SuperClass::m_data.find(v);
			if (itr == __SuperClass::m_data.end())
			{
				return 0;
			}

			__SuperClass::m_data.erase(itr);

			return 1;
		}

	public:
		template <typename T>
		JSSet<T, __Container> map(__FN_CB<__DataType, T> fn) const
		{
			JSSet<T, __Container> ret;

			for (auto& v : __SuperClass::m_data)
			{
				ret._add(fn(v));
			}

			return ret;
		}

		JSSet filter(__JSSet_Cond fn) const
		{
			JSSet ret;

			for (auto& v : __SuperClass::m_data)
			{
				if (fn(v))
				{
					ret._add(v);
				}
			}

			return ret;
		}
	};

	template <typename __DataType>
	using JSUnsortSet = JSSet<__DataType, unordered_set>;
}

#endif // __JSSet_H
