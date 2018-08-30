
#ifndef __PtrArray_H
#define __PtrArray_H

#include "JSArray.h"

namespace NS_JSTL
{
	template<typename __Type>
	class PtrArray : public JSArray<__Type*>
	{
	private:
		using __PtrType = __Type*;
		using __RefType = __Type&;

		using __SuperClass = JSArray<__PtrType>;

		using __PtrArray_InitList = __InitList<__PtrType>;

	public:
		PtrArray()
		{
		}

		template<typename... args>
		explicit PtrArray(__PtrType ptr, args... others)
			: __SuperClass(ptr, others...)
		{
		}

		template<typename... args>
		explicit PtrArray(__RefType ref, args&... others)
		{
			assign(ref, others...);
		}
		
		explicit PtrArray(const PtrArray& arr)
			: __SuperClass(arr)
		{
		}

		PtrArray(PtrArray&& arr)
		{
			__SuperClass::swap(arr);
		}

		explicit PtrArray(__PtrArray_InitList initList)
			: __SuperClass(initList)
		{
		}

		template<typename T, typename _ITR = decltype(declval<T>().begin())>
		explicit PtrArray(const T& container)
		{
			assign(container);
		}

		PtrArray& operator=(const PtrArray& arr)
		{
			__SuperClass::assign(arr);
			return *this;
		}

		PtrArray& operator=(PtrArray&& arr)
		{
			__SuperClass::swap(arr);
			return *this;
		}

		PtrArray& operator=(__PtrArray_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		PtrArray& operator=(const T&t)
		{
			assign(t);
			return *this;
		}
		
	private:
		TD_SizeType _add(const __PtrType& ptr) override
		{
			return __SuperClass::_add(ptr);
		}

		TD_SizeType _add(__RefType ref)
		{
			return __SuperClass::_add(&ref);
		}
		
		void _unshift(const __PtrType& ptr)
		{
			__SuperClass::unshift(ptr);
		}

		void _unshift(__RefType ref)
		{
			__SuperClass::unshift(&ref);
		}

	public:
		template<typename... args>
		PtrArray& assign(__PtrType ptr, args... others)
		{
			__SuperClass::assign(ptr, others...);
			return *this;
		}

		template<typename... args>
		PtrArray& assign(__RefType ref, args&... others)
		{
			__SuperClass::clear();

			push(ref, others...);

			return *this;
		}

		PtrArray& assign(__PtrArray_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		PtrArray& assign(const T& container)
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return *this;
			}

			__SuperClass::clear();

			push(container);

			return *this;
		}

		template<typename... args>
		TD_SizeType push(__PtrType ptr, args... others)
		{
			return __SuperClass::push(ptr, others...);
		}

		template<typename... args>
		TD_SizeType push(__RefType ref, args&... others)
		{
			(void)tagDynamicArgsExtractor<__RefType>::extract([&](__RefType ref) {
				_add(ref);
				return true;
			}, ref, others...);

			return __SuperClass::size();
		}

		template<typename T>
		decltype(checkContainer<T, TD_SizeType>()) push(const T& container)
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return __SuperClass::size();
			}

			for (auto&v : container)
			{
				_add(v);
			}
			
			return __SuperClass::size();
		}

		TD_SizeType push(__PtrArray_InitList initList)
		{
			return __SuperClass::push(initList);
		}

		template<typename... args>
		PtrArray concat(__PtrType ptr, args... others) const
		{
			return PtrArray(__SuperClass::concat(ptr, others...));
		}

		template<typename... args>
		PtrArray concat(__RefType ref, args&... others)
		{
			PtrArray arr(*this);
			arr.push(ref, others...);
			return arr;
		}

		template<typename T>
		PtrArray concat(const T& container) const
		{
			PtrArray arr(*this);
			arr.push(container);
			return arr;
		}

		PtrArray concat(__PtrArray_InitList initList) const
		{
			return PtrArray(__SuperClass::concat(initList));
		}

		template<typename... args>
		TD_SizeType unshift(__PtrType ptr, args... others)
		{
			return __SuperClass::unshift(ptr, others...);
		}

		template<typename... args>
		TD_SizeType unshift(__RefType ref, args&... others)
		{
			(void)tagDynamicArgsExtractor<__RefType>::extract([&](__RefType ref) {
				__SuperClass::unshift({ &ref });
				return true;
			}, ref, others...);

			return __SuperClass::size();
		}
		
		template<typename T>
		decltype(checkContainer<T, TD_SizeType>()) unshift(const T& container)
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return __SuperClass::size();
			}

			for (auto&v : container)
			{
				_unshift(v);
			}

			return __SuperClass::size();
		}

		TD_SizeType unshift(__PtrArray_InitList initList)
		{
			return __SuperClass::unshift(initList);
		}

		template<typename... args>
		PtrArray& splice(TD_PosType pos, TD_SizeType nRemove, __PtrType v, args... others)
		{
			__SuperClass::splice(pos, nRemove, v, others...);

			return *this;
		}

		template<typename... args>
		PtrArray& splice(TD_PosType pos, TD_SizeType nRemove, __RefType ref, args&... others)
		{
			vector<__PtrType> vec;
			(void)tagDynamicArgsExtractor<__RefType>::extract([&](__RefType ref) {
				vec.push_back(&ref);
				return true;
			}, ref, others...);

			__SuperClass::splice(pos, nRemove, vec);

			return *this;
		}

		template<typename T>
		PtrArray& splice(TD_PosType pos, TD_SizeType nRemove = 0, const T& container = {})
		{
			__SuperClass::splice(pos, nRemove, container);

			return *this;
		}

		PtrArray& splice(TD_PosType pos, TD_SizeType nRemove, __PtrArray_InitList initList)
		{
			__SuperClass::splice(pos, nRemove, initList);

			return *this;
		}
	};

	template<typename __Type>
	class ConstPtrArray : public PtrArray<const __Type>
	{
	private:
		using __SuperClass = PtrArray<const __Type>;

		using __ConstPtrType = const __Type*;
		using __ConstRefType = const __Type&;

		using __ConstPtrArray_InitList = __InitList<__ConstPtrType>;

	public:
		ConstPtrArray()
		{
		}

		template<typename... args>
		explicit ConstPtrArray(__ConstPtrType ptr, args... others)
			: __SuperClass(ptr, others...)
		{
		}

		template<typename... args>
		explicit ConstPtrArray(__ConstRefType ref, const args&... others)
		{
			assign(ref, others...);
		}

		explicit ConstPtrArray(const ConstPtrArray& arr)
			: __SuperClass(arr)
		{
		}

		ConstPtrArray(ConstPtrArray&& arr)
		{
			__SuperClass::swap(arr);
		}

		explicit ConstPtrArray(__ConstPtrArray_InitList initList)
			: __SuperClass(initList)
		{
		}

		template<typename T, typename _ITR = decltype(declval<T>().begin())>
		explicit ConstPtrArray(const T& container)
			: __SuperClass(container)
		{
		}

		ConstPtrArray& operator=(const ConstPtrArray& arr)
		{
			__SuperClass::assign(arr);
			return *this;
		}

		ConstPtrArray& operator=(ConstPtrArray&& arr)
		{
			__SuperClass::swap(arr);
			return *this;
		}

		ConstPtrArray& operator=(__ConstPtrArray_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		ConstPtrArray& operator=(const T&t)
		{
			__SuperClass::assign(t);
			return *this;
		}
	};
}

#endif // __PtrArray_H
