
#ifndef __JSMap_H
#define __JSMap_H

#include "Container.h"

#include <unordered_map>
#include <map>

#include "JSArray.h"

namespace NS_JSTL
{
	template<typename __KeyType, typename __ValueType, template<typename...> typename __MapType>
	class JSMap : public ContainerT<pair<__KeyType const, __ValueType>, __MapType<__KeyType, __ValueType>, __KeyType>
	{
	private:
		using __SuperClass = ContainerT<pair<__KeyType const, __ValueType>, __MapType<__KeyType, __ValueType>, __KeyType>;
		
		using __DataType = pair<__KeyType const, __ValueType>;
		using __JSMap_InitList = __InitList<__DataType>;

		using __ConstKeyRef = const __KeyType&;
		using __ConstValueRef = const __ValueType&;

		template <typename T>
		using __CB_T = const function<T(__ConstKeyRef, __ConstValueRef)>&;

		using __CB_void = __CB_T<void>;
		using __CB_bool = __CB_T<bool>;

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

		explicit JSMap(__JSMap_InitList initList)
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

		JSMap& operator=(__JSMap_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template<typename T>
		JSMap& operator=(const T&t)
		{
			__SuperClass::assign(t);
			return *this;
		}

	private:
		TD_SizeType _add(const __DataType& pr) override
		{
			__SuperClass::m_data.insert(pr);

			return __SuperClass::m_data.size();
		}

		TD_SizeType _del(__ConstKeyRef key) override
		{
			auto itr = __SuperClass::m_data.find(key);
			if (itr == __SuperClass::m_data.end())
			{
				return 0;
			}

			__SuperClass::m_data.erase(itr);

			return 1;
		}

		bool _includes(__ConstKeyRef key) const override
		{
			return __SuperClass::m_data.find(key) != __SuperClass::m_data.end();
		}
		
		virtual void _tostring(stringstream& ss, const __DataType& pr) const override
		{
			tagSSTryLMove(ss) << '<' << pr.first << ", " << pr.second << '>';
		}

	public:
		__ValueType& operator[](__ConstKeyRef key)
		{
			return __SuperClass::m_data[key];
		}
		__ConstValueRef operator[](__ConstKeyRef key)const
		{
			return __SuperClass::m_data[key];
		}

		bool get(__ConstKeyRef key, __CB_void fn=NULL) const
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

		bool find(__CB_bool fn)
		{
			if (!fn)
			{
				return false;
			}

			for (auto& pr : __SuperClass::m_data)
			{
				if (fn(pr.first, pr.second))
				{
					return true;
				}
			}

			return false;
		}

		JSArray<__KeyType> keys(__CB_bool fn=NULL) const
		{
			JSArray<__KeyType> arr;
			for (auto& pr : __SuperClass::m_data)
			{
				if (fn)
				{
					if (!fn(pr.first, pr.second))
					{
						continue;
					}
				}

				arr.push(pr.first);
			}

			return arr;
		}

		JSArray<__ValueType> values(__CB_bool fn = NULL) const
		{
			JSArray<__ValueType> arr;
			for (auto& pr : __SuperClass::m_data)
			{
				if (fn)
				{
					if (!fn(pr.first, pr.second))
					{
						continue;
					}
				}

				arr.push(pr.second);
			}

			return arr;
		}

		JSMap& set(__ConstKeyRef key, __ConstValueRef value)
		{
			__SuperClass::m_data[key] = value;
			return *this;
		}

		template<typename T>
		TD_SizeType set(const T& container)
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return __SuperClass::size();
			}

			__SuperClass::m_data.insert(container.begin(), container.end());

			return __SuperClass::m_data.size();
		}

		TD_SizeType set(__JSMap_InitList initList)
		{
			return set<__JSMap_InitList>(initList);
		}

	public:
		template <typename T>
		JSMap<__KeyType, T, __MapType> map(__CB_T<T> fn) const
		{
			JSMap<__KeyType, T, __MapType> ret;

			if (fn)
			{
				for (auto& pr : __SuperClass::m_data)
				{
					ret.set(pr.first, fn(pr.first, pr.second));
				}
			}

			return ret;
		}

		template <typename __Function, typename __RET = decltype(declval<__Function>()(__KeyType(), __ValueType()))>
		JSMap<__KeyType, __RET, __MapType> map(__Function fn) const
		{
			return map<__RET>(fn);
		}

		JSMap filter(__CB_bool fn) const
		{
			JSMap ret;

			for (auto& pr : __SuperClass::m_data)
			{
				if (fn(pr.first, pr.second))
				{
					ret.set(pr.first, pr.second);
				}
			}

			return ret;
		}
	};

	template <typename __KeyType, typename __ValueType>
	using JSUnsortMap = JSMap<__KeyType, __ValueType, unordered_map>;
}

#endif // __JSMap_H
