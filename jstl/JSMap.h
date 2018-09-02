
#ifndef __JSMap_H
#define __JSMap_H

#include "Container.h"

#include <unordered_map>
#include <map>

#include "JSArray.h"
 
namespace NS_JSTL
{
	template<typename __DataType> class JSArray;

	template<typename __KeyType, typename __ValueType, template<typename...> typename __MapType> class JSMapT
		: public ContainerT<pair<__KeyType const, __ValueType>, __MapType<__KeyType, __ValueType>, __KeyType>
	{
	protected:
		using __ContainerType = __MapType<__KeyType, __ValueType>;
		using __SuperClass = ContainerT<pair<__KeyType const, __ValueType>, __ContainerType, __KeyType>;

		using __DataType = pair<__KeyType const, __ValueType>;
		using __JSMap_InitList = __InitList<__DataType>;

		using __ConstKeyRef = const __KeyType&;

		using __ValueRef = __ValueType&;
		using __ConstValueRef = const __ValueType&;

		template <typename T>
		using __CB_CRK_RV_T = const function<T(__ConstKeyRef, __ValueRef)>&;
		using __CB_CRK_RV_void = __CB_CRK_RV_T<void>;
		using __CB_CRK_RV_bool = __CB_CRK_RV_T<bool>;

		template <typename T>
		using __CB_CRK_CRV_T = const function<T(__ConstKeyRef, __ConstValueRef)>&;
		using __CB_CRK_CRV_void = __CB_CRK_CRV_T<void>;
		using __CB_CRK_CRV_bool = __CB_CRK_CRV_T<bool>;

		using __CB_RV_CRK_void = const function<void(__ValueRef, __ConstKeyRef)>&;
		using __CB_CRV_CRK_void = const function<void(__ConstValueRef, __ConstKeyRef)>&;
		using __CB_CRV_CRK_bool = const function<bool(__ConstValueRef, __ConstKeyRef)>&;

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
		JSMapT()
		{
		}

		explicit JSMapT(const JSMapT& map)
			: __SuperClass(map)
		{
		}

		JSMapT(JSMapT&& map)
		{
			__SuperClass::swap(map);
		}

		explicit JSMapT(__JSMap_InitList initList)
			: __SuperClass(initList)
		{
		}

		template<typename T>
		explicit JSMapT(const T& container)
			: __SuperClass(container)
		{
		}

		JSMapT& operator=(const JSMapT& map)
		{
			__SuperClass::assign(map);
			return *this;
		}

		JSMapT& operator=(JSMapT&& map)
		{
			__SuperClass::swap(map);
			return *this;
		}

		JSMapT& operator=(__JSMap_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template<typename T>
		JSMapT& operator=(const T&t)
		{
			__SuperClass::assign(t);
			return *this;
		}

	protected:
		TD_SizeType _add(const __DataType& pr) override
		{
			_data().insert(pr);

			return _data().size();
		}

		TD_SizeType _del(__ConstKeyRef key) override
		{
			auto itr = _data().find(key);
			if (itr == _data().end())
			{
				return 0;
			}

			_data().erase(itr);

			return 1;
		}

		bool _includes(__ConstKeyRef key) const override
		{
			return _data().find(key) != _data().end();
		}

		virtual void _toString(stringstream& ss, const __DataType& pr) const override
		{
			tagSSTryLMove(ss) << '<' << pr.first << ", " << pr.second << '>';
		}

	public:
		bool get(__ConstKeyRef key, __CB_RV_CRK_void cb)
		{
			auto itr = _data().find(key);
			if (itr == _data().end())
			{
				return false;
			}

			if (cb)
			{
				cb(itr->second, itr->first);
			}

			return true;
		}

		bool get(__ConstKeyRef key, __CB_CRV_CRK_void cb) const
		{
			auto itr = _data().find(key);
			if (itr == _data().end())
			{
				return false;
			}

			if (cb)
			{
				cb(itr->second, itr->first);
			}

			return true;
		}

		bool findValue(__CB_CRV_CRK_bool cb) const
		{
			if (!cb)
			{
				return false;
			}

			for (auto& pr : _data())
			{
				if (cb(pr.second, pr.first))
				{
					return true;
				}
			}

			return false;
		}
		
		JSArray<__KeyType> keys(__CB_CRK_CRV_bool cb = NULL) const
		{
			JSArray<__KeyType> arr;
			for (auto& pr : _data())
			{
				if (cb)
				{
					if (!cb(pr.first, pr.second))
					{
						continue;
					}
				}

				arr.push(pr.first);
			}

			return arr;
		}

		JSArray<__ValueType> values(__CB_CRK_CRV_bool cb = NULL) const
		{
			JSArray<__ValueType> arr;
			for (auto& pr : _data())
			{
				if (cb)
				{
					if (!cb(pr.first, pr.second))
					{
						continue;
					}
				}

				arr.push(pr.second);
			}

			return arr;
		}

		JSMapT& set(__ConstKeyRef key, __ConstValueRef value)
		{
			_data()[key] = value;
			return *this;
		}

		template<typename T>
		TD_SizeType set(const T& container)
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return __SuperClass::size();
			}

			_data().insert(container.begin(), container.end());

			return _data().size();
		}

		TD_SizeType set(__JSMap_InitList initList)
		{
			return set<__JSMap_InitList>(initList);
		}

	public:
		template <typename T>
		JSMapT<__KeyType, T, __MapType> map(__CB_CRK_CRV_T<T> cb) const
		{
			JSMapT<__KeyType, T, __MapType> ret;

			if (cb)
			{
				for (auto& pr : _data())
				{
					ret.set(pr.first, cb(pr.first, pr.second));
				}
			}

			return ret;
		}

		template <typename CB, typename __RET = decltype(declval<CB>()(__KeyType(), __ValueType()))>
		JSMapT<__KeyType, __RET, __MapType> map(const CB& cb) const
		{
			return map<__RET>(cb);
		}

		JSMapT filter(__CB_CRK_CRV_bool cb) const
		{
			JSMapT ret;

			if (cb)
			{
				for (auto& pr : _data())
				{
					if (cb(pr.first, pr.second))
					{
						ret.set(pr.first, pr.second);
					}
				}
			}

			return ret;
		}
	};

	template <typename __KeyType, typename __ValueType>
	using JSMap = JSMapT<__KeyType, __ValueType, map>;

	template <typename __KeyType, typename __ValueType>
	using JSUnsortMap = JSMapT<__KeyType, __ValueType, unordered_map>;
}

#endif // __JSMap_H
