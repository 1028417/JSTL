
#ifndef __ArrList_H
#define __ArrList_H

namespace NS_SSTL
{
#define __ArrListSuper SListT<__DataType, __BaseType>

	template<typename __DataType, template<typename...> class __BaseType>
	class ArrListT : public __ArrListSuper
	{
	private:
		__UsingSuper(__ArrListSuper)

		__UsingSuperType(__RItrType)
		__UsingSuperType(__CRItrType)

		PtrArray<__DataType> m_ptrArray;
		
	public:
		ArrListT() = default;

		template<typename... args>
		explicit ArrListT(__DataConstRef data, const args&... others)
			: __Super(data, others...)
		{
		}

		explicit ArrListT(__ContainerType&& container)
		{
			__Super::swap(container);
		}

		ArrListT(ArrListT&& lst)
		{
			__Super::swap(lst);
		}

		ArrListT(const ArrListT& lst)
			: __Super(lst)
			, m_ptrArray(m_data)
		{
		}

		explicit ArrListT(__InitList initList)
			: __Super(initList)
			, m_ptrArray(m_data)
		{
		}

		template<typename T, typename = checkContainerData_t<T, __DataType>>
		explicit ArrListT(const T& container)
			: __Super(container)
			, m_ptrArray(m_data)
		{
		}

		ArrListT& operator=(__ContainerType&& container)
		{
			__Super::swap(container);
			return *this;
		}

		ArrListT& operator=(ArrListT&& lst)
		{
			__Super::swap(lst);
			return *this;
		}

		ArrListT& operator=(const ArrListT& lst)
		{
			__Super::assign(lst);
			return *this;
		}

		ArrListT& operator=(__InitList initList)
		{
			__Super::assign(initList);
			return *this;
		}

		template <typename T>
		ArrListT& operator=(const T&t)
		{
			__Super::assign(t);
			return *this;
		}

		template <typename T>
		ArrListT& operator=(T&t)
		{
			__Super::assign(t);
			return *this;
		}

		template<typename CB>
		void operator() (const CB& cb, size_t startPos = 0, size_t count = 0)
		{
			adaptor().forEach(cb, startPos, count);
		}

		template<typename CB>
		void operator() (const CB& cb, size_t startPos = 0, size_t count = 0) const
		{
			adaptor().forEach(cb, startPos, count);
		}

	public:
		const __ContainerType& operator->() const
		{
			return m_data;
		}

		void clear() override
		{
			m_ptrArray.clear();
			m_data.clear();
		}

		const __ContainerType& data() const
		{
			return m_data;
		}

		operator __ContainerType& () = delete;

		operator const __ContainerType& ()
		{
			return m_data;
		}
		operator const __ContainerType& () const
		{
			return m_data;
		}

#ifndef __MINGW32__
		virtual __ItrType erase(const __CItrType& itr)
		{
			m_ptrArray.del(&*itr);

			return m_data.erase(itr);
		}
#else
		virtual __ItrType erase(const __ItrType& itr)
		{
            m_ptrArray.del(&*itr);

            return m_data.erase(itr);
		}
#endif

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

	public:
		int find(__CB_ConstRef_bool cb, size_t stratPos = 0) const
		{
			int iRetPos = -1;

			(*this)([&](__DataConstRef data, size_t pos) {
				if (cb(data))
				{
					iRetPos = pos;
					return false;
				}

				return true;
			});

			return iRetPos;
		}

		bool get(size_t pos, __CB_Ref_void cb)
		{
			return m_ptrArray.get(pos, cb);
		}

		bool get(size_t pos, __CB_ConstRef_void cb) const
		{
			return m_ptrArray.get(pos, cb);
		}

		bool get(size_t pos, __DataRef& data) const
		{
			return m_ptrArray.get(pos, [&](__DataConstRef& t_data) {
				data = t_data;
			});
		}

		bool set(size_t pos, __DataConstRef& data)
		{
			return get([&](__DataRef m_data) {
				m_data = data;
			});
		}

		template<typename T>
		ArrListT& addFront(const T& container)
		{
			if (!__Super::checkIsSelf(container))
			{
				itrReverseVisit(container, [&](__DataConstRef data) {
					m_data.push_front(data);
					m_ptrArray.addFront(m_data.front());
				});
			}

			return *this;
		}

		ArrListT& addFront(__InitList initList)
		{
			addFront<__InitList>(initList);
		}

		template<typename... args>
		ArrListT& addFront(__DataConstRef data, const args&... others)
		{
			(void)tagDynamicArgsExtractor<const __DataType>::extractReverse([&](__DataConstRef data) {
				m_data.push_front(data);
				m_ptrArray.addFront(m_data.front());
				return true;
			}, data, others...);

			return *this;
		}

		bool popBack(__CB_Ref_void cb = NULL)
		{
			(void)m_ptrArray.popBack();

			return __Super::popBack(cb);
		}

		bool popBack(__DataRef data)
		{
			(void)m_ptrArray.popBack();

			return __Super::popBack(data);
		}

		ArrListT& sort(__CB_Sort_T<__DataType> cb = NULL)
		{
			__Super::sort(cb);
			m_ptrArray.assign(m_data);

			return *this;
		}

		ArrListT& Reverse()
		{
			reverse(m_data.begin(), m_data.end());
			m_ptrArray.Reverse();

			return *this;
		}

		ArrListT slice(int startPos, int endPos=-1) const
		{
			ArrListT lst;

			auto ptrArray = m_ptrArray.slice(startPos, endPos);
			ptrArray([&](__DataConstRef data) {
				lst.add(data);
			});

			return lst;
		}

	private:
		virtual void _swap(__ContainerType& container)
		{
			if (&container != &m_data)
			{
				m_data.swap(container);
				m_ptrArray.assign(m_data);
			}
		}
		
		void _add(__DataConstRef data) override
		{
			m_data.push_back(data);
			m_ptrArray.add(m_data.back());
		}

		bool _popFront(__CB_Ref_void cb = NULL) override
		{
			(void)m_ptrArray.popFront();
			return __Super::_popFront(cb);
		}

		size_t _del(__DataConstRef data, CB_Del cb) override
		{
			size_t uRet = 0;

			auto itrPtr = m_ptrArray.begin();
			for (auto itr = m_data.begin(); itr != m_data.end() && itrPtr != m_ptrArray.end(); )
			{
				if (!tagTryCompare<__DataConstRef>::compare(*itr, data))
				{
					++itr;
					++itrPtr;
					continue;
				}

				E_DelConfirm eRet = cb(*itr);
				if (E_DelConfirm::DC_Abort == eRet)
				{
					break;
				}
				else if (E_DelConfirm::DC_No == eRet)
				{
					++itr;
					++itrPtr;
					continue;
				}
				else
				{
					itr = __Super::erase(itr);
					itrPtr = m_ptrArray.erase(itrPtr);
					uRet++;

					if (E_DelConfirm::DC_YesAbort == eRet)
					{
						break;
					}
				}
			}

			return uRet;
		}

	private:
		template <class __DataRef>
		class CAdaptor
		{
		public:
			CAdaptor(const PtrArray<__DataType>& ptrArray)
				: m_ptrArray(ptrArray)
			{
			}

		private:
			const PtrArray<__DataType>& m_ptrArray;

		public:
			template <typename CB, typename = checkCBBool_t<CB, __DataRef, size_t>>
			void forEach(const CB& cb, size_t startPos = 0, size_t count = 0) const
			{
				m_ptrArray(cb);
			}

			template <typename CB, typename = checkCBBool_t<CB, __DataRef>, typename = void>
			void forEach(const CB& cb, size_t startPos = 0, size_t count = 0) const
			{
				m_ptrArray(cb);
			}

			template <typename CB, typename = checkCBVoid_t<CB, __DataRef>, typename = void, typename = void>
			void forEach(const CB& cb, size_t startPos = 0, size_t count = 0) const
			{
				m_ptrArray(cb);
			}

			template <typename CB, typename = checkCBVoid_t<CB, __DataRef, size_t>, typename = void, typename = void, typename = void>
			void forEach(const CB& cb, size_t startPos = 0, size_t count = 0) const
			{
				m_ptrArray(cb);
			}
		};

		CAdaptor<__DataRef> m_adaptor = CAdaptor<__DataRef>(m_ptrArray);
		CAdaptor<__DataRef>& adaptor()
		{
			return m_adaptor;
		}
		CAdaptor<__DataConstRef>& adaptor() const
		{
			return (CAdaptor<__DataConstRef>&)m_adaptor;
		}
	};
}

#endif //__ArrList_H