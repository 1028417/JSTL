
#ifndef __SContainer_H
#define __SContainer_H

#include <sstream>

#include <list>

namespace NS_SSTL
{
	template<typename __ContainerType__, typename __KeyType__>
	class SContainerT
	{
	protected:
		using __ContainerType = __ContainerType__;

		using __DataType = containerDataType_t<__ContainerType>;
		using __DataRef = containerRefType_t<__ContainerType>;
		using __DataConstRef = const __DataType&;

		using __ItrType = containerItrType_t<__ContainerType>;
		using __CItrType = containerCItrType_t<__ContainerType>;

		using CB_Del = CB_T_Ret<__DataRef, E_DelConfirm>;

		using __InitList = InitList_T<__DataType>;

		using __CB_Ref_void = CB_T_void<__DataRef>;
		using __CB_Ref_bool = CB_T_bool<__DataRef>;

		using __CB_ConstRef_void = CB_T_void<__DataConstRef>;
		using __CB_ConstRef_bool = CB_T_bool<__DataConstRef>;

		using __KeyType = __KeyType__;
		using __KeyConstRef = const __KeyType&;
		using __InitList_Key = InitList_T<__KeyType>;

		__ContainerType m_data;

	public:
		SContainerT() = default;

		template<typename... args>
		explicit SContainerT(__DataConstRef data, const args&... others)
		{
			add(data, others...);
		}

		explicit SContainerT(__ContainerType&& container)
		{
			_swap(container);
		}

		SContainerT(SContainerT&& container)
		{
			swap(container);
		}

		SContainerT(const SContainerT& container)
			: m_data(container.begin(), container.end())
		{
		}

		explicit SContainerT(__InitList initList)
			: m_data(initList.begin(), initList.end())
		{
		}

		template<typename T, typename = checkContainer_t<T>>
		explicit SContainerT(const T& container)
			: m_data(container.begin(), container.end())
		{
		}

		SContainerT& operator=(__ContainerType&& container)
		{
			swap(container);
			return *this;
		}

		SContainerT& operator=(SContainerT&& container)
		{
			swap(container);
			return *this;
		}

		SContainerT& operator=(const SContainerT& container)
		{
			assign(container);
			return *this;
		}

		SContainerT& operator=(__InitList initList)
		{
			assign(initList);
			return *this;
		}

		template <typename T>
		SContainerT& operator=(const T&t)
		{
			assign(t);
			return *this;
		}

		template <typename T>
		SContainerT& operator=(T&t)
		{
			assign(t);
			return *this;
		}

		SContainerT& operator+= (__DataConstRef data)
		{
			add(data);
			return *this;
		}

		template <typename T>
		SContainerT& operator+= (const T& rhs)
		{
			add(rhs);
			return *this;
		}

		SContainerT& operator+= (__InitList rhs)
		{
			add(rhs);
			return *this;
		}

		SContainerT& operator-= (__KeyConstRef key)
		{
			del(key);
			return *this;
		}

		template <typename T>
		SContainerT& operator-= (const T& container)
		{
			del(container);
			return *this;
		}

		SContainerT& operator-= (__InitList_Key keys)
		{
			del(keys);
			return *this;
		}

		friend SContainerT operator+ (const SContainerT& lhs, const SContainerT& rhs)
		{
			return SContainerT(lhs) += rhs;
		}

		friend SContainerT operator+ (const SContainerT& lhs, __InitList rhs)
		{
			return SContainerT(lhs) += rhs;
		}

		friend SContainerT operator+ (__InitList lhs, const SContainerT& rhs)
		{
			return SContainerT(lhs) += rhs;
		}

		template <typename T>
		friend SContainerT operator+ (const SContainerT& lhs, const T& rhs)
		{
			return SContainerT(lhs) += rhs;
		}

		template <typename T, typename = checkNotIsBase_t<SContainerT, T>>
		friend SContainerT operator+ (const T& lhs, const SContainerT& rhs)
		{
			return SContainerT(lhs) += rhs;
		}

		friend SContainerT operator- (const SContainerT& lhs, const SContainerT& rhs)
		{
			return SContainerT(lhs) -= rhs;
		}

		friend SContainerT operator- (const SContainerT& lhs, __InitList rhs)
		{
			return SContainerT(lhs) -= rhs;
		}

		friend SContainerT operator- (__InitList lhs, const SContainerT& rhs)
		{
			return SContainerT(lhs) -= rhs;
		}

		template <typename T>
		friend SContainerT operator- (const SContainerT& lhs, const T& rhs)
		{
			return SContainerT(lhs) -= rhs;
		}

		template <typename T, typename = checkNotIsBase_t<SContainerT, T>>
		friend SContainerT operator- (const T& lhs, const SContainerT& rhs)
		{
			return SContainerT(lhs) -= rhs;
		}

		template <typename T>
		friend SContainerT operator& (const SContainerT& lhs, const T& rhs)
		{
			SContainerT ret;
			for (auto&data : rhs)
			{
				if (lhs.includes(data))
				{
					ret.add(data);
				}
			}

			return ret;
		}

		friend SContainerT operator& (const SContainerT& lhs, __InitList rhs)
		{
			return lhs & SContainerT(rhs);
		}

		__ContainerType& operator->()
		{
			return m_data;
		}
		const __ContainerType& operator->() const
		{
			return m_data;
		}

		operator __ContainerType& ()
		{
			return m_data;
		}

		operator const __ContainerType& () const
		{
			return m_data;
		}
		__ContainerType& data()
		{
			return m_data;
		}
		const __ContainerType& data() const
		{
			return m_data;
		}

		operator bool() const
		{
			return !m_data.empty();
		}

		bool empty() const
		{
			return m_data.empty();
		}

		size_t size() const
		{
			return m_data.size();
		}

		virtual void clear()
		{
			m_data.clear();
        }

#ifndef __MINGW32__
		virtual __ItrType erase(const __CItrType& itr)
		{
			return m_data.erase(itr);
		}
#else
		virtual __ItrType erase(const __ItrType& itr)
		{
			return m_data.erase(itr);
		}
#endif

		__ItrType begin()
		{
			return m_data.begin();
		}
		__CItrType begin() const
		{
			return m_data.begin();
		}

		__ItrType end()
		{
			return m_data.end();
		}
		__CItrType end() const
		{
			return m_data.end();
		}
		
	public:
		SContainerT& swap(SContainerT& container)
		{
			_swap(container.m_data);
			return *this;
		}

		SContainerT& swap(__ContainerType& container)
		{
			_swap(container);
			return *this;
		}

		template<typename... args>
		SContainerT& assign(__DataConstRef data, const args&... others)
		{
			clear();

			add(data, others...);

			return *this;
		}

		SContainerT assign(__ContainerType&& container)
		{
			_swap(container);
		}

		SContainerT& assign(SContainerT&& container)
		{
			return swap(container);
		}

		SContainerT& assign(__InitList initList)
		{
			return assign<__InitList>(initList);
		}

		template<typename T, typename = checkContainer_t<T>>
		SContainerT& assign(const T& container)
		{
			if (checkIsSelf(container))
			{
				return *this;
			}

			//clear();
			
			__ContainerType tmp(container.begin(), container.end());
			_swap(tmp);

			return *this;
		}

		template<typename... args>
		void add(__DataConstRef data, const args&... others)
		{
			(void)extractDataTypeArgs([&](__DataConstRef data) {
				_add(data);
				return true;
			}, data, others...);
		}

		void add(__InitList initList)
		{
			add<__InitList>(initList);
		}

		template<typename T, typename = checkContainer_t<T>>
		void add(const T& container)
		{
			if (checkIsSelf(container))
			{
				return;
			}

			for (auto&data : container)
			{
				_add(data);
			}
		}

		bool getFront(__CB_Ref_void cb)
		{
			if (m_data.empty())
			{
				return false;
			}

			cb(*m_data.begin());

			return true;
		}

		bool getFront(__CB_ConstRef_void cb) const
		{
			if (m_data.empty())
			{
				return false;
			}

			cb(*m_data.begin());

			return true;
		}

		bool getFront(__DataRef data) const
		{
			if (m_data.empty())
			{
				return false;
			}

			data = *m_data.begin();

			return true;
		}

		bool getBack(__CB_Ref_void cb)
		{
			if (m_data.empty())
			{
				return false;
			}

			cb(*m_data.rbegin());

			return true;
		}

		bool getBack(__CB_ConstRef_void cb) const
		{
			if (m_data.empty())
			{
				return false;
			}

			cb(*m_data.rbegin());
		}

		bool getBack(__DataRef data) const
		{
			if (m_data.empty())
			{
				return false;
			}

			data = *m_data.rbegin();

			return true;
		}

		bool popFront(__CB_Ref_void cb = NULL)
		{
			return _popFront(cb);
		}

		bool popFront(__DataRef data)
		{
			return _popFront([&](__DataRef t_data) {
				data = t_data;
			});
		}

		bool includes(__KeyConstRef key) const
		{
			return _includes(key);
		}

		template<typename... args>
		bool includes(__KeyConstRef data, const args&... others) const
		{
			return tagDynamicArgsExtractor<__KeyConstRef>::extract([&](__KeyConstRef data) {
				return _includes(data);
			}, data, others...);
		}

		template<typename T, typename = checkContainer_t<T>>
		bool includes(const T& container) const
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

		bool includes(__InitList_Key initList) const
		{
			return includes<__InitList_Key>(initList);
		}

		bool del(__KeyConstRef key, bool bOnlyOne = false)
		{
			return 1==_del(key, [&](__DataRef data) {
				if (bOnlyOne)
				{
					return E_DelConfirm::DC_YesAbort;
				}

				return E_DelConfirm::DC_Yes;
			});
		}

		size_t del(__KeyConstRef key, __CB_Ref_void cb)
		{
			return _del(key, [&](__DataRef data) {
				cb(data);
				
				return E_DelConfirm::DC_Yes;
			});
		}

		size_t del_if(__KeyConstRef key, __CB_Ref_bool cb)
		{
			return _del(key, [&](__DataRef data) {
				if (cb(data))
				{
					return E_DelConfirm::DC_Yes;
				}
				else
				{
					return E_DelConfirm::DC_No;
				}
			});
		}

		size_t del_if(__KeyConstRef key, CB_Del cb)
		{
			return _del(key, cb);
		}
		
		size_t del_ex(CB_Del cb)
		{
			size_t uRet = 0;

			for (auto itr = m_data.begin(); itr != m_data.end();)
			{
				E_DelConfirm ret = cb(*itr);
				if (E_DelConfirm::DC_Abort == ret)
				{
					break;
				}
				else if (E_DelConfirm::DC_No == ret)
				{
					++itr;
				}
				else
				{
					itr = erase(itr);
					uRet++;

					if (E_DelConfirm::DC_YesAbort == ret)
					{
						break;
					}
				}
			}

			return uRet;
		}

		size_t del_ex(__CB_Ref_bool cb)
		{
			size_t uRet = 0;

			for (auto itr = m_data.begin(); itr != m_data.end();)
			{
				if (cb(*itr))
				{
					itr = erase(itr);
					uRet++;
				}
				else
				{
					++itr;
				}
			}

			return uRet;
		}

		template<typename... args, typename = checkArgs0Type_t<__KeyType, args...>>
		size_t del(__KeyConstRef key, const args&... others)
		{
			size_t uRet = 0;

			(void)extractKeyTypeArgs([&](__KeyConstRef key) {
				if (m_data.empty())
				{
					return false;
				}

				uRet += del(key);
				return true;
			}, key, others...);

			return uRet;
		}

		template<typename T, typename = checkContainer_t<T>>
		size_t del(const T& container)
		{
			if (checkIsSelf(container))
			{
				size_t uRet = m_data.size();
				clear();
				return uRet;
			}

			size_t uRet = 0;

			for (auto& key : container)
			{
				if (m_data.empty())
				{
					break;
				}

				uRet += del(key);
			}

			return uRet;
		}

		size_t del(__InitList_Key keys)
		{
			return del<__InitList_Key>(keys);
		}

	public:
		template<typename... args>
		SContainerT concat(__DataConstRef data, const args&... others) const
		{
			SContainerT ret(*this);
			ret.add(data, others...);
			return ret;
		}

		SContainerT concat(__InitList initList) const
		{
			return concat<__InitList>(initList);
		}

		template<typename T>
		SContainerT concat(const T& container) const
		{
			SContainerT ret(*this);
			ret.add(container);
			return ret;
		}

	public:
		SContainerT map(CB_T_Ret<__DataConstRef, __DataType> cb) const
		{
			SContainerT ret;

			for (auto&data : m_data)
			{
				ret.add(cb(data));
			}

			return ret;
		}

		SContainerT filter(__CB_ConstRef_bool cb) const
		{
			SContainerT ret;

			for (auto&data : m_data)
			{
				if (cb(data))
				{
					ret.add(data);
				}
			}

			return ret;
		}

		bool every(__CB_ConstRef_bool cb) const
		{
			for (auto&data : m_data)
			{
				if (!cb(data))
				{
					return false;
				}
			}

			return true;
		}

		bool any(__CB_ConstRef_bool cb) const
		{
			for (auto&data : m_data)
			{
				if (cb(data))
				{
					return true;
				}
			}

			return false;
		}

		__DataType reduce(const function<__DataType(__DataConstRef, __DataConstRef)>& cb) const
		{
			return NS_SSTL::reduce<__DataType, __ContainerType >(m_data, cb);
		}

		string toString(const string& strSplitor = ",") const
		{
			stringstream ss;
			ss << '[';
			for (auto itr = m_data.begin(); itr != m_data.end(); ++itr)
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
		template<typename T>
		bool checkIsSelf(const T& container) const
		{
			if (is_same<T, __ContainerType>::value)
			{
				return (__ContainerType*)&container == &m_data;
			}

			return (SContainerT*)&container == this;
		}

		template<typename... args>
		static bool extractDataTypeArgs(__CB_ConstRef_bool cb, __DataConstRef data, const args&... others)
		{
			return tagDynamicArgsExtractor<const __DataType>::extract([&](__DataConstRef data) {
				return cb(data);
			}, data, others...);
		}

		template<typename... args>
		static bool extractKeyTypeArgs(CB_T_bool<__KeyConstRef> cb, __KeyConstRef key, const args&... others)
		{
			return tagDynamicArgsExtractor<const __KeyType>::extract([&](__KeyConstRef key) {
				return cb(key);
			}, key, others...);
		}

		template<typename... args>
		static void extractKeyTypeArgs(vector<__KeyType>& vecArgs, __KeyConstRef key, const args&... others)
		{
			extractKeyTypeArgs([&](__KeyConstRef key) {
				vecArgs.push_back(key);
				return true;
			}, key, others...);
		}

	private:
		virtual void _swap(__ContainerType& container)
		{
			m_data.swap(container);
		}

		virtual void _add(__DataConstRef data)
		{
			m_data.insert(m_data.end(), data);
		}

		virtual bool _popFront(__CB_Ref_void cb = NULL)
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

			erase(itr);

			return true;
		}

		virtual size_t _del(__KeyConstRef key, CB_Del cb) { return 0; }

		virtual bool _includes(__KeyConstRef key) const { return false; }

		virtual void _toString(stringstream& ss, __DataConstRef data) const
		{
			tagSSTryLMove(ss) << data;
		}
	};
}

#endif //__SContainer_H
