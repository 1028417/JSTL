
#ifndef __JSMap_H
#define __JSMap_H

#include "Container.h"

#include <unordered_map>
#include <map>

#include "JSArray.h"

namespace NS_JSTL
{
	template<typename __KeyType, typename __ValueType, template<typename...> typename __Container>
	class JSMap : public ContainerT<pair<__KeyType const, __ValueType>, __Container<__KeyType, __ValueType>, __KeyType>
	{
	private:
		using __SuperClass = ContainerT<pair<__KeyType const, __ValueType>, __Container<__KeyType, __ValueType>, __KeyType>;
		
		using __DataType = pair<__KeyType const, __ValueType>;
		using __JSMap_InitList = __InitList<__DataType>;

		using __FN_CB = const function<void(const __KeyType&key, const __ValueType&v)>&;
		using __FN_Cond = const function<bool(const __KeyType&key, const __ValueType&v)>&;

	public:
		JSMap()
		{
		}

		explicit JSMap(const JSMap& map)
			: __SuperClass(map)
		{
		}

		JSMap(JSMap&& map)
		{
			__SuperClass::swap(map);
		}

		explicit JSMap(const __JSMap_InitList& initList)
			: __SuperClass(initList)
		{
		}

		template<typename T>
		explicit JSMap(const T& container)
			: __SuperClass(container)
		{
		}

		JSMap& operator=(const JSMap& map)
		{
			__SuperClass::assign(map);
			return *this;
		}

		JSMap& operator=(JSMap&& map)
		{
			__SuperClass::swap(map);
			return *this;
		}

		JSMap& operator=(const __JSMap_InitList& initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template<typename T>
		JSMap& operator=(const T& container)
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

		virtual void _tostring(stringstream& ss, const __DataType&v) const override
		{
			tagSSTryLMove(ss) << '<' << v.first << ", " << v.second << '>';
		}

	public:
		__ValueType& operator[](const __KeyType&key)
		{
			return __SuperClass::m_data[key];
		}
		const __ValueType& operator[](const __KeyType&key)const
		{
			return __SuperClass::m_data[key];
		}

		bool has(const __KeyType&key) const override
		{
			return __SuperClass::m_data.find(key) != __SuperClass::m_data.end();
		}

		bool get(const __KeyType&key, __FN_CB fn=NULL) const
		{
			auto itr = __SuperClass::m_data.find(key);
			if (itr == __SuperClass::m_data.end())
			{
				return false;
			}

			if (fn)
			{
				fn(itr->first, itr->second);
			}

			return true;
		}

		bool find(__FN_Cond fn)
		{
			for (auto& pr : __SuperClass::m_data)
			{
				if (fn(pr.first, pr.second))
				{
					return true;
				}
			}

			return false;
		}

		JSArray<__KeyType> keys(__FN_Cond fn=NULL) const
		{
			JSArray<__KeyType> ret;
			for (auto& pr : __SuperClass::m_data)
			{
				if (!fn || fn(pr.first, pr.second))
				{
					ret.push(pr.first);
				}
			}

			return ret;
		}

		JSArray<__ValueType> values(__FN_Cond fn = NULL) const
		{
			JSArray<__ValueType> ret;
			for (auto& pr : __SuperClass::m_data)
			{
				if (!fn || fn(pr.first, pr.second))
				{
					ret.push(pr.second);
				}
			}

			return ret;
		}

		JSMap& set(const __KeyType&key, const __ValueType& val)
		{
			__SuperClass::m_data[key] = val;
			return *this;
		}

		template<typename T>
		TD_SizeType set(const T& container)
		{
			if (__SuperClass::CheckIsSelf(container))
			{
				return __SuperClass::size();
			}

			__SuperClass::m_data.insert(container.begin(), container.end());

			return __SuperClass::m_data.size();
		}

		TD_SizeType set(const __JSMap_InitList& initList)
		{
			return set<__JSMap_InitList>(initList);
		}

		template<typename T>
		JSMap concat(const T& container)
		{
			JSMap ret(*this);
			ret.set(container);
			return ret;
		}

		JSMap concat(const __JSMap_InitList& initList)
		{
			JSMap ret(*this);
			ret.set(initList);
			return ret;
		}

		TD_SizeType del(const __KeyType&key) override
		{
			auto itr = __SuperClass::m_data.find(key);
			if (itr == __SuperClass::m_data.end())
			{
				return 0;
			}

			__SuperClass::m_data.erase(itr);

			return 1;
		}

	public:
		template <typename __Function>
		auto map(__Function fn) const ->JSMap<__KeyType, decltype(fn(__KeyType(), __ValueType())), __Container>
		{
			JSMap<__KeyType, decltype(fn(__KeyType(), __ValueType())), __Container> ret;

			for (auto& pr : __SuperClass::m_data)
			{
				ret.set(pr.first, fn(pr.first, pr.second));
			}

			return ret;
		}

		template <typename __Function>
		auto mapArray(__Function fn) const ->JSMap<__KeyType, decltype(fn(__KeyType(), __ValueType())), __Container>
		{
			JSArray<decltype(fn(__KeyType(), __ValueType()))> ret;

			for (auto& pr : __SuperClass::m_data)
			{
				ret.push(fn(pr.first, pr.second));
			}

			return ret;
		}

		JSMap filter(__FN_Cond fn) const
		{
			JSMap ret;

			for (auto& pr : __SuperClass::m_data)
			{
				if (fn(pr.first, pr.second))
				{
					ret._add(pr);
				}
			}

			return ret;
		}
	};

	template <typename __KeyType, typename __ValueType>
	using JSUnsortMap = JSMap<__KeyType, __ValueType, unordered_map>;
}

#endif // __JSMap_H
