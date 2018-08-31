
#ifndef __ContainerType_H
#define __ContainerType_H

#include "util.h"

namespace NS_JSTL
{
	template<typename __DataType> class JSArray;

	template<typename __DataType, typename __ContainerType, typename __KeyType = __DataType>
	class ContainerT
	{
	private:
		using __Data_InitList = __InitList<__DataType>;
		using __Key_InitList = __InitList<__KeyType>;

		using __Container_CB = __FN_CB<__DataType>;
		using __Container_CB_RetBool = __FN_CB_RetBool<__DataType>;

		using __ConstDataRef = const __DataType&;

		using __Data_Pos_CB = const function<bool(__ConstDataRef, TD_PosType)>&;

	protected:
		__ContainerType m_data;

		template<typename... args>
		bool extractDataTypeArgs(const function<bool(__ConstDataRef)>& cb, __ConstDataRef data, const args&... others)
		{
			return tagDynamicArgsExtractor<const __DataType>::extract([&](__ConstDataRef data) {
				return cb(data);
			}, data, others...);
		}

		template<typename... args>
		void extractDataTypeArgs(vector<__DataType>& vecArgs, __ConstDataRef data, const args&... others)
		{
			tagDynamicArgsExtractor<const __DataType>::extract([&](__ConstDataRef data) {
				vecArgs.push_back(data);
				return true;
			}, data, others...);
		}

		template<typename... args>
		bool extractKeyTypeArgs(const function<bool(const __KeyType&)>& cb, const __KeyType&k, const args&... others)
		{
			return tagDynamicArgsExtractor<const __KeyType>::extract([&](const __KeyType&k) {
				return cb(k);
			}, k, others...);
		}

		template<typename... args>
		void extractKeyTypeArgs(vector<__KeyType>& vecArgs, const __KeyType&k, const args&... others)
		{
			tagDynamicArgsExtractor<const __KeyType>::extract([&](const __KeyType&k) {
				vecArgs.push_back(k);
				return true;
			}, k, others...);
		}

		ContainerT()
		{
		}

		template<typename T>
		explicit ContainerT(const T& container)
			: m_data(container.begin(), container.end())
		{
		}

		template<typename T>
		bool checkIsSelf(const T& container)
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

		bool front(__Container_CB fn=NULL) const
		{
			auto itr = m_data.begin();
			if (itr == m_data.end())
			{
				return false;
			}

			if (fn)
			{
				fn(*itr);
			}

			return true;
		}

		bool back(__Container_CB fn=NULL) const
		{
			auto itr = m_data.rbegin();
			if (itr == m_data.rend())
			{
				return false;
			}

			if (fn)
			{
				fn(*itr);
			}

			return true;
		}

		bool pop_begin(__Container_CB fn=NULL)
		{
			auto itr = m_data.begin();
			if (itr == m_data.end())
			{
				return false;
			}

			if (fn)
			{
				fn(*itr);
			}
			m_data.erase(itr);

			return true;
		}

		virtual bool _includes(const __KeyType&k) const = 0;

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
		vector<__KeyType> getInner(const __KeyType&k, const args&... others)
		{
			vector<__KeyType> vec;
			extractKeyTypeArgs(vec, k, others...);
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
		vector<__KeyType> getOuter(const __KeyType&k, const args&... others)
		{
			vector<__KeyType> vec;
			extractKeyTypeArgs(vec, k, others...);
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
		TD_SizeType del(const __KeyType&k, const args&... others)
		{
			TD_SizeType uRet = 0;

			(void)extractKeyTypeArgs([&](const __KeyType&k) {
				uRet += _del(k);
				return true;
			}, k, others...);

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

		TD_SizeType del(__Container_CB_RetBool fn)
		{
			TD_SizeType uRet = 0;

			for (auto itr = m_data.begin(); itr != m_data.end();)
			{
				if (fn(*itr))
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

		string toString(const string& strSplitor = ",")
		{
			stringstream ss;
			ss << '[';
			for (auto itr = m_data.begin(); itr != m_data.end(); itr++)
			{
				if (itr != m_data.begin())
				{
					ss << strSplitor.c_str();
				}

				_tostring(ss, *itr);
			}

			ss << ']';
			return ss.str();
		}

		const __ContainerType& data() const
		{
			return m_data;
		}
		__ContainerType& data()
		{
			return m_data;
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

		virtual TD_SizeType _del(const __KeyType&k) = 0;

		virtual void _tostring(stringstream& ss, __ConstDataRef data) const
		{
			tagSSTryLMove(ss) << data;
		}

	public:
		template <typename T>
		JSArray<T> map(__FN_CB<__DataType, T> fn) const
		{
			JSArray<T> arr;

			for (auto&data : m_data)
			{
				arr.push(fn(data));
			}

			return arr;
		}

		template <typename __Function>
		auto map(__Function fn) const ->JSArray<decltype(fn(__DataType()))> const
		{
			JSArray<decltype(fn(__DataType()))> arr;

			for (auto&data : m_data)
			{
				arr.push(fn(data));
			}

			return arr;
		}

		void forEach(__Data_Pos_CB fn) const
		{
			if (!fn)
			{
				return;
			}

			TD_PosType pos = 0;
			for (auto&data : m_data)
			{
				if (!fn(pos, data))
				{
					break;
				}

				pos++;
			}
		}

		bool every(__Container_CB_RetBool fn)
		{
			for (auto&data : m_data)
			{
				if (!fn(data))
				{
					return false;
				}
			}

			return true;
		}

		bool some(__Container_CB_RetBool fn)
		{
			for (auto&data : m_data)
			{
				if (fn(data))
				{
					return true;
				}
			}

			return false;
		}

		template<typename T, typename __Function>
		T reduce(const T& stat, __Function fn)
		{
			T ret = stat;
			for (auto&data : m_data)
			{
				ret = (T)fn(ret, data);
			}

			return ret;
		}
	};
}

#endif //__ContainerType_H
