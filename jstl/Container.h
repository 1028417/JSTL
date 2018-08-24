
#ifndef __Container_H
#define __Container_H

#include "util.h"

namespace NS_JSTL
{
	template<typename __DataType, typename __Container, typename __KeyType = __DataType>
	class ContainerT
	{
	private:
		using __Data_InitList = __InitList<__DataType>;
		using __Key_InitList = __InitList<__KeyType>;

		using __Container_CB = __FN_CB<__DataType>;
		using __Container_Cond = __FN_Cond<__DataType>;

	protected:
		__Container m_data;

		ContainerT()
		{
		}

		template<typename T>
		explicit ContainerT(const T& container)
			: m_data(container.begin(), container.end())
		{
		}

	public:
		template<typename T>
		ContainerT& assign(const T& container)
		{
			if ((void*)this != (void*)&container)
			{
				m_data = __Container(container.begin(), container.end());

				//clear();
				//for (auto& v : container)
				//{
				//	this->_add(v);
				//}
			}

			return *this;
		}

		ContainerT& assign(const __Data_InitList& initList)
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

		virtual bool has(const __KeyType& key) const = 0;

		template<typename T>
		bool includes(const T& container) const
		{
			for (auto& v : container)
			{
				if (!has(v))
				{
					return false;
				}
			}

			return true;
		}

		template<typename T>
		bool includes(const __Data_InitList& initList) const
		{
			return includes<__Data_InitList>(initList);
		}

		template <typename T>
		T getInner(const T& container) const
		{
			T ret(container);

			if (!m_data.empty())
			{
				for (auto itr = ret.begin(); itr != ret.end(); )
				{
					if (!has(*itr))
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

		vector<__KeyType> getInner(const __Key_InitList& initList) const
		{
			vector<__KeyType > vec(initList);
			return getInner(vec);
		}

		template <typename T>
		T getOuter(const T& container) const
		{
			T ret(container);

			if (!m_data.empty())
			{
				for (auto itr = ret.begin(); itr != ret.end(); )
				{
					if (has(*itr))
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

		vector<__KeyType> getOuter(const __Key_InitList& initList) const
		{
			vector<__KeyType> vec(initList);
			return getOuter(vec);
		}

		template <typename T>
		T erase(T& itr)
		{
			return itr = m_data.erase(itr);
		}

		virtual TD_SizeType remove(const __KeyType& key) = 0;

		template <typename T>
		TD_SizeType remove(const T& container)
		{
			TD_SizeType uRet = 0;

			for (auto& v : container)
			{
				if (m_data.empty())
				{
					break;
				}

				uRet += remove(v);
			}

			return uRet;
		}

		TD_SizeType remove(const __Key_InitList& initList)
		{
			return remove<__Key_InitList>(initList);
		}

		TD_SizeType remove(__Container_Cond fn)
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

		const __Container& data() const
		{
			return m_data;
		}
		__Container& data()
		{
			return m_data;
		}

	private:
		virtual TD_SizeType _add(const __DataType&v) = 0;

		virtual void _tostring(stringstream& ss, const __DataType&v) const
		{
			tagSSTryLMove(ss) << v;
		}

	public:
		void forEach(function<bool(TD_PosType pos, const __DataType&v)> fn) const
		{
			TD_PosType pos = 0;
			for (auto& v : m_data)
			{
				if (!fn(pos, v))
				{
					break;
				}

				pos++;
			}
		}

		bool every(__Container_Cond fn)
		{
			for (auto& v : m_data)
			{
				if (!fn(v))
				{
					return false;
				}
			}

			return true;
		}

		bool some(__Container_Cond fn)
		{
			for (auto& v : m_data)
			{
				if (fn(v))
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
			for (auto& v : m_data)
			{
				ret = (T)fn(ret, v);
			}

			return ret;
		}
	};
}

#endif //__Container_H
