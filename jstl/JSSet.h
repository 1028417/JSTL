
#ifndef __JSSet_H
#define __JSSet_H

#include "Container.h"

#include <unordered_set>
#include <set>

namespace NS_JSTL
{
	template<typename __DataType, template<typename...> class __Container=set
		, typename __SueperClass = ContainerT<__DataType, __Container<__DataType>>
	> class JSSet : public __SueperClass
	{
	private:
		using __JSSet_InitList = __InitList<__DataType>;

		using __JSSet_CB = __FN_CB<__DataType>;
		using __JSSet_Cond = __FN_Cond<__DataType>;

	public:
		JSSet()
		{
		}

		explicit JSSet(const JSSet& set)
			: __SueperClass(set)
		{
		}

		JSSet(JSSet&& set)
		{
			__SueperClass::swap(set);
		}

		explicit JSSet(const __JSSet_InitList& initList)
			: __SueperClass(initList)
		{
		}

		template<typename T>
		explicit JSSet(const T& container)
			: __SueperClass(container)
		{
		}

		JSSet& operator=(const JSSet& set)
		{
			__SueperClass::assign(set);
			return *this;
		}

		JSSet& operator=(JSSet&& set)
		{
			__SueperClass::swap(set);
			return *this;
		}

		JSSet& operator=(const __JSSet_InitList& initList)
		{
			__SueperClass::assign(initList);
			return *this;
		}

		template <typename T>
		JSSet& operator=(const T& container)
		{
			__SueperClass::assign(container);
			return *this;
		}
		
	private:
		virtual TD_SizeType _add(const __DataType&v) override
		{
			__SueperClass::m_data.insert(v);

			return __SueperClass::m_data.size();
		}
		
	public:
		bool has(const __DataType&v) const override
		{
			return __SueperClass::m_data.find(v) != __SueperClass::m_data.end();
		}

		TD_SizeType add(const __DataType&v)
		{
			return _add(v);
		}

		template<typename T>
		TD_SizeType add(const T& container)
		{
			__SueperClass::m_data.insert(container.begin(), container.end());

			return __SueperClass::m_data.size();
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

		TD_SizeType remove(const __DataType&v) override
		{
			auto itr = __SueperClass::m_data.find(v);
			if (itr == __SueperClass::m_data.end())
			{
				return 0;
			}

			__SueperClass::m_data.erase(itr);

			return 1;
		}

	public:
		template <typename T>
		JSSet<T, __Container> map(__FN_CB<__DataType, T> fn) const
		{
			JSSet<T, __Container> ret;

			for (auto& v : __SueperClass::m_data)
			{
				ret._add(fn(v));
			}

			return ret;
		}

		JSSet filter(__JSSet_Cond fn) const
		{
			JSSet ret;

			for (auto& v : __SueperClass::m_data)
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
