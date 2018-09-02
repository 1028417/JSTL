
#ifndef __ContainerType_H
#define __ContainerType_H

#include "util.h"

#include <vector>
#include <sstream>

#include <initializer_list>
#include <algorithm>

using namespace std;

namespace NS_JSTL
{
	typedef size_t TD_PosType;
	typedef size_t TD_SizeType;

	template <typename T>
	using __InitList = const initializer_list<T>&;

	template <typename T, typename __RET>
	using __CB_T_RET = const function<__RET(const T&v)>&;

	template <typename T>
	using __CB_T_void = __CB_T_RET<T, void>;

	template <typename T>
	using __CB_T_bool = __CB_T_RET<T, bool>;

	template <typename T>
	using __CB_Data_Pos = const function<bool(T, TD_PosType)>&;

	template <typename DATA>
	class __ContainerOperator
	{
	public:
		__ContainerOperator(DATA& data)
			: m_data(data)
		{
		}

	protected:
		DATA& m_data;
		
	public:
		template <typename CB>
		bool getFront(const CB& cb)
		{
			auto itr = m_data.begin();
			if (itr == m_data.end())
			{
				return false;
			}

			if (cb)
			{
				cb(*itr);
			}

			return true;
		}

		template <typename CB>
		bool getBack(const CB& cb)
		{
			auto itr = m_data.rbegin();
			if (itr == m_data.rend())
			{
				return false;
			}

			if (cb)
			{
				cb(*itr);
			}

			return true;
		}

		template <typename CB>
		size_t forEach(const CB& cb)
		{
			if (!cb)
			{
				return 0;
			}

			size_t pos = 0;
			for (auto&data : m_data)
			{
				if (!cb(data, pos))
				{
					break;
				}

				pos++;
			}

			return pos;
		}
	};


	template<typename __DataType, typename __ContainerType, typename __KeyType = __DataType>
	class ContainerT
	{
	protected:
		using __Data_InitList = __InitList<__DataType>;
		using __Key_InitList = __InitList<__KeyType>;

		using __DataRef = __DataType&;
		using __ConstDataRef = const __DataType&;
		using __ConstKeyRef = const __KeyType&;

		using __CB_Ref_void = __CB_T_void<__DataRef>;
		using __CB_Ref_bool = __CB_T_bool<__DataRef>;
		using __CB_Ref_Pos = __CB_Data_Pos<__DataRef>;

		using __CB_ConstRef_void = __CB_T_void<__ConstDataRef>;
		using __CB_ConstRef_bool = __CB_T_bool<__ConstDataRef>;
		using __CB_ConstRef_Pos = __CB_Data_Pos<__ConstDataRef>;

	protected:
		ContainerT()
			: m_ContainerOperator(m_data)
			, m_ContainerReader(m_data)
		{
		}

		template<typename T>
		explicit ContainerT(const T& container)
			: m_data(container.begin(), container.end())
			, m_ContainerOperator(m_data)
			, m_ContainerReader(m_data)
		{
		}

		__ContainerType m_data;

	protected:
		using __ContainerOperatorType = __ContainerOperator<__ContainerType>;
		__ContainerOperatorType m_ContainerOperator;
		using __ContainerReaderType = __ContainerOperator<const __ContainerType>;
		__ContainerReaderType m_ContainerReader;

	protected:
		__ContainerOperatorType& getContainerOperator()
		{
			return m_ContainerOperator;
		}

		__ContainerReaderType& getContainerOperator() const
		{
			return (__ContainerReaderType&)m_ContainerReader;
		}

		template<typename... args>
		static bool extractDataTypeArgs(__CB_ConstRef_bool cb, __ConstDataRef data, const args&... others)
		{
			return tagDynamicArgsExtractor<const __DataType>::extract([&](__ConstDataRef data) {
				return cb(data);
			}, data, others...);
		}

		template<typename... args>
		static void extractDataTypeArgs(vector<__DataType>& vecArgs, __ConstDataRef data, const args&... others)
		{
			extractDataTypeArgs([&](__ConstDataRef data) {
				vecArgs.push_back(data);
				return true;
			}, data, others...);
		}
		
		template<typename... args>
		static bool extractKeyTypeArgs(__CB_T_bool<__ConstKeyRef> cb, __ConstKeyRef key, const args&... others)
		{
			return tagDynamicArgsExtractor<const __KeyType>::extract([&](__ConstKeyRef key) {
				return cb(key);
			}, key, others...);
		}

		template<typename... args>
		static void extractKeyTypeArgs(vector<__KeyType>& vecArgs, __ConstKeyRef key, const args&... others)
		{
			extractKeyTypeArgs([&](__ConstKeyRef key) {
				vecArgs.push_back(key);
				return true;
			}, key, others...);
		}

		template<typename T>
		bool checkIsSelf(const T& container) const
		{
			return ((void*)&container == (void*)this) || ((void*)&container == (void*)&m_data);
		}

	public:
		template<typename... args>
		ContainerT& assign(__ConstDataRef data, const args&... others)
		{
			clear();

			add(data, others...);

			return *this;
		}

		template<typename T>
		ContainerT& assign(const T& container)
		{
			if (checkIsSelf(container))
			{
				return *this;
			}

			m_data = __ContainerType(container.begin(), container.end());

			return *this;
		}

		ContainerT& assign(__Data_InitList initList)
		{
			return assign<__Data_InitList>(initList);
		}

		ContainerT& swap(ContainerT& container)
		{
			if ((void*)this != (void*)&container)
			{
				m_data.swap(container.m_data);
			}

			return *this;
		}

	public:
		TD_SizeType size() const
		{
			return m_data.size();
		}

		TD_SizeType length() const
		{
			return m_data.size();
		}

		decltype(m_data.begin()) begin()
		{
			return m_data.begin();
		}
		decltype(m_data.end()) end()
		{
			return m_data.end();
		}

		decltype(m_data.cbegin()) begin() const
		{
			return m_data.cbegin();
		}
		decltype(m_data.cbegin()) end() const
		{
			return m_data.cend();
		}

		bool getFront(__CB_Ref_void cb)
		{
			return getContainerOperator().getFront(cb);
		}

		bool getFront(__CB_ConstRef_void cb) const
		{
			return getContainerOperator().getFront(cb);
		}

		bool getFront(__DataRef data) const
		{
			return getContainerOperator().getFront([&](__ConstDataRef _data) {
				data = _data;
			});
		}

		void getBack(__CB_Ref_void cb)
		{
			getContainerOperator().getBack(cb);
		}

		void getBack(__CB_ConstRef_void cb) const
		{
			getContainerOperator().getBack(cb);
		}

		bool getBack(__DataRef data) const
		{
			return getContainerOperator().getBack([&](__ConstDataRef _data) {
				data = _data;
			});
		}

		bool popFront(__CB_ConstRef_void cb = NULL)
		{
			auto itr = m_data.begin();
			if (itr == m_data.end())
			{
				return false;
			}

			if (cb)
			{
				cb(*itr);
			}
			m_data.erase(itr);

			return true;
		}

		bool popFront(__DataRef data)
		{
			auto itr = m_data.begin();
			if (itr == m_data.end())
			{
				return false;
			}

			data = *itr;
			m_data.erase(itr);

			return true;
		}

		virtual bool _includes(__ConstKeyRef key) const = 0;

		template<typename... args>
		bool includes(__ConstDataRef data, const args&... others)
		{
			bool bRet = true;

			(void)extractDataTypeArgs([&](__ConstDataRef data) {
				return bRet = _includes(data);
			}, data, others...);

			return bRet;
		}

		template<typename T>
		decltype(checkContainer<T, bool>()) includes(const T& container) const
		{
			if (checkIsSelf(container))
			{
				return true;
			}

			for (auto&data : container)
			{
				if (!_includes(data))
				{
					return false;
				}
			}

			return true;
		}

		bool includes(__Data_InitList initList) const
		{
			return includes<__Data_InitList>(initList);
		}

		template<typename... args>
		vector<__KeyType> getInner(__ConstKeyRef key, const args&... others) const
		{
			vector<__KeyType> vec;
			extractKeyTypeArgs(vec, key, others...);
			return getInner(vec);
		}

		template <typename T>
		T getInner(const T& container) const
		{
			T ret(container);

			if (!m_data.empty())
			{
				for (auto itr = ret.begin(); itr != ret.end(); )
				{
					if (!_includes(*itr))
					{
						itr = ret.erase(itr);
					}
					else
					{
						itr++;
					}
				}
			}

			return ret;
		}

		vector<__KeyType> getInner(__Key_InitList initList) const
		{
			vector<__KeyType> vec(initList);
			return getInner(vec);
		}

		template<typename... args>
		vector<__KeyType> getOuter(__ConstKeyRef key, const args&... others) const
		{
			vector<__KeyType> vec;
			extractKeyTypeArgs(vec, key, others...);
			return getOuter(vec);
		}

		template <typename T>
		T getOuter(const T& container) const
		{
			T ret(container);

			if (!m_data.empty())
			{
				for (auto itr = ret.begin(); itr != ret.end(); )
				{
					if (_includes(*itr))
					{
						itr = ret.erase(itr);
					}
					else
					{
						itr++;
					}
				}
			}

			return ret;
		}

		vector<__KeyType> getOuter(__Key_InitList initList) const
		{
			vector<__KeyType> vec(initList);
			return getOuter(vec);
		}

		template <typename T>
		T erase(T& itr)
		{
			return itr = m_data.erase(itr);
		}

		template<typename... args>
		TD_SizeType del(__ConstKeyRef key, const args&... others)
		{
			TD_SizeType uRet = 0;

			(void)extractKeyTypeArgs([&](__ConstKeyRef key) {
				uRet += _del(key);
				return true;
			}, key, others...);

			return uRet;
		}

		template <typename T>
		decltype(checkContainer<T, TD_SizeType>()) del(const T& container)
		{
			if (checkIsSelf(container))
			{
				TD_SizeType uRet = this->size();
				this->clear();
				return uRet;
			}

			TD_SizeType uRet = 0;

			for (auto&data : container)
			{
				if (m_data.empty())
				{
					break;
				}

				uRet += _del(data);
			}

			return uRet;
		}

		TD_SizeType del(__Key_InitList initList)
		{
			return del<__Key_InitList>(initList);
		}

		TD_SizeType del(__CB_ConstRef_bool cb)
		{
			if (!cb)
			{
				return 0;
			}

			TD_SizeType uRet = 0;

			for (auto itr = m_data.begin(); itr != m_data.end();)
			{
				if (cb(*itr))
				{
					itr = m_data.erase(itr);
					uRet++;
				}
				else
				{
					itr++;
				}
			}

			return uRet;
		}

		void clear()
		{
			m_data.clear();
		}

		string toString(const string& strSplitor = ",") const
		{
			stringstream ss;
			ss << '[';
			for (auto itr = m_data.begin(); itr != m_data.end(); itr++)
			{
				if (itr != m_data.begin())
				{
					ss << strSplitor.c_str();
				}

				_toString(ss, *itr);
			}

			ss << ']';
			return ss.str();
		}
		
	protected:
		virtual TD_SizeType _add(__ConstDataRef data) = 0;

		template<typename... args>
		TD_SizeType add(__ConstDataRef data, const args&... others)
		{
			(void)extractDataTypeArgs([&](__ConstDataRef data) {
				_add(data);
				return true;
			}, data, others...);

			return size();
		}

		template<typename T>
		decltype(checkContainer<T, TD_SizeType>()) add(const T& container)
		{
			if (checkIsSelf(container))
			{
				return size();
			}

			for (auto&data : container)
			{
				_add(data);
			}

			return size();
		}

		TD_SizeType add(__Data_InitList initList)
		{
			return add<__Data_InitList>(initList);
		}

		virtual TD_SizeType _del(__ConstKeyRef key) = 0;

		virtual void _toString(stringstream& ss, __ConstDataRef data) const
		{
			tagSSTryLMove(ss) << data;
		}

	public:
		TD_SizeType forEach(__CB_Ref_Pos cb)
		{
			return getContainerOperator().forEach(cb);
		}

		TD_SizeType forEach(__CB_ConstRef_Pos cb) const
		{
			return getContainerOperator().forEach(cb);
		}

		bool every(__CB_ConstRef_bool cb) const
		{
			if (!cb)
			{
				return false;
			}

			for (auto&data : m_data)
			{
				if (!cb(data))
				{
					return false;
				}
			}

			return true;
		}

		bool some(__CB_ConstRef_bool cb) const
		{
			if (!cb)
			{
				return false;
			}

			for (auto&data : m_data)
			{
				if (cb(data))
				{
					return true;
				}
			}

			return false;
		}

		template<typename T, typename CB>
		T reduce(const T& stat, const CB& cb) const
		{
			T ret = stat;
			for (auto&data : m_data)
			{
				ret = (T)cb(ret, data);
			}

			return ret;
		}
	};
}

#endif //__ContainerType_H
