
#ifndef __SList_H
#define __SList_H

namespace NS_SSTL
{
	template<typename __DataType, template<typename...> class __BaseType>
	class SListT : public __SuperT
	{
	private:
		__UsingSuper(__SuperT);
		
	private:
		template <class T = __ContainerType>
		class __ListOperator
		{
		public:
			__ListOperator(T& data)
				: m_data(data)
			{
			}

		private:
			T& m_data;

			using __RefType = decltype(m_data.front())&;

		public:
			template <typename CB, typename = checkCBBool_t<CB, __RefType>>
			void forEach(const CB& cb)
			{
				for (auto& data : m_data)
				{
					if (!cb(data))
					{
						break;
					}
				}
			}

			template <typename CB, typename = checkCBVoid_t<CB, __RefType>, typename = void>
			void forEach(const CB& cb)
			{
				forEach([&](__RefType data) {
					cb(data);
					return true;
				});
			}
		};

		__ListOperator<> m_ListOperator = __ListOperator<>(m_data);
		__ListOperator<>& _getOperator()
		{
			return m_ListOperator;
		}

		__ListOperator<const __ContainerType>& _getOperator() const
		{
			return (__ListOperator<const __ContainerType>&)m_ListOperator;
		}

	public:
		SListT() = default;

		template<typename... args>
		explicit SListT(__DataConstRef data, const args&... others)
			: __Super(data, others...)
		{
		}

		explicit SListT(__ContainerType&& container)
		{
			__Super::swap(container);
		}

		SListT(SListT&& lst)
		{
			__Super::swap(lst);
		}

		SListT(const SListT& lst)
			: __Super(lst)
		{
		}

		explicit SListT(__InitList initList)
			: __Super(initList)
		{
		}

		template<typename T, typename = checkContainerData_t<T, __DataType>>
		explicit SListT(const T& container)
			: __Super(container)
		{
		}

		SListT& operator=(__ContainerType&& container)
		{
			__Super::swap(container);
			return *this;
		}

		SListT& operator=(SListT&& lst)
		{
			__Super::swap(lst);
			return *this;
		}

		SListT& operator=(const SListT& lst)
		{
			__Super::assign(lst);
			return *this;
		}

		SListT& operator=(__InitList initList)
		{
			__Super::assign(initList);
			return *this;
		}

		template <typename T>
		SListT& operator=(const T&t)
		{
			__Super::assign(t);
			return *this;
		}

		template <typename T>
		SListT& operator=(T&t)
		{
			__Super::assign(t);
			return *this;
		}

		template <typename T>
		friend SListT operator& (const SListT& lhs, const T& rhs)
		{
			SListT lst;
			for (auto&data : rhs)
			{
				if (lhs.includes(data))
				{
					lst.add(data);
				}
			}

			return lst;
		}

		friend SListT operator& (const SListT& lhs, __InitList rhs)
		{
			return lhs & SListT(rhs);
		}

		template<typename CB>
		void operator() (const CB& cb)
		{
			_getOperator().forEach(cb);
		}

		template<typename CB>
		void operator() (const CB& cb) const
		{
			_getOperator().forEach(cb);
		}

	public:
		__RItrType rbegin()
		{
			return m_data.rbegin();
		}
		__CRItrType rbegin() const
		{
			return m_data.rbegin();
		}

		__RItrType rend()
		{
			return m_data.rend();
		}
		__CRItrType rend() const
		{
			return m_data.rend();
		}

		template<typename T>
		SListT& addFront(const T& container)
		{
			__Super::addFront(container);
			return *this;
		}

		SListT& addFront(__InitList initList)
		{
			__Super::addFront(initList);
			return *this;
		}

		template<typename... args>
		SListT& addFront(__DataConstRef data, const args&... others)
		{
			(void)tagDynamicArgsExtractor<const __DataType>::extractReverse([&](__DataConstRef data) {
				m_data.push_front(data);
				return true;
			}, data, others...);

			return *this;
		}

		bool popBack(__CB_ConstRef_void cb = NULL)
		{
			if (m_data.empty())
			{
				return false;
			}

			if (cb)
			{
				cb(m_data.back());
			}

			m_data.pop_back();

			return true;
		}

		bool popBack(__DataRef data)
		{
			if (m_data.empty())
			{
				return false;
			}

			data = m_data.back();
			m_data.pop_back();

			return true;
		}
		
	public:
		SListT& sort(__CB_Sort_T<__DataType> cb = NULL)
		{
			if (cb)
			{
				m_data.sort(tagTrySort<__DataType>(cb));
			}
			else
			{
				m_data.sort();
			}

			return *this;
		}

		SListT& Reverse()
		{
			reverse(m_data.begin(), m_data.end());

			return *this;
		}

	public:
		template <typename T>
		SListT<T, __BaseType> map(CB_T_Ret<__DataConstRef, T> cb) const
		{
			SListT<T, __BaseType> lst;

			for (auto&data : m_data)
			{
				lst.add(cb(data));
			}

			return lst;
		}

		template <typename CB, typename RET = decltype(declval<CB>()(__DataType()))>
		SListT<RET, __BaseType> map(const CB& cb) const
		{
			return map<RET>(cb);
		}

		SMap<__DataType, size_t> itemSum() const
		{
			SMap<__DataType, size_t> mapItemSum;

			for (auto&data : m_data)
			{
				mapItemSum[data]++;
			}

			return mapItemSum;
		}

		void sum(SMap<__DataType, size_t>& mapItemSum, SMap<size_t, SListT>& mapSumItem) const
		{
			mapItemSum = itemSum();

			for (auto& pr : mapItemSum)
			{
				mapSumItem[pr.second].add(pr.first);
			}
		}

		SMap<size_t, SListT> sumItem() const
		{
			SMap<__DataType, size_t> mapItemSum;
			SMap<size_t, SListT> mapSumItem;
			sum(mapItemSum, mapSumItem);
			return mapSumItem;
		}

	private:
		inline void _add(__DataConstRef data) override
		{
			m_data.push_back(data);
		}

		bool _popFront(__CB_Ref_void cb = NULL) override
		{
			if (m_data.empty())
			{
				return false;
			}

			if (cb)
			{
				cb(m_data.front());
			}

			m_data.pop_front();

			return true;
		}

		size_t _find(__DataConstRef data, const CB_Find& cb = NULL) override
		{
			return NS_SSTL::find(m_data, data, cb);
		}

		size_t _cfind(__DataConstRef data, const CB_ConstFind& cb = NULL) const override
		{
			return NS_SSTL::find(m_data, data, cb);
		}
	};
}

#endif // __SList_H
