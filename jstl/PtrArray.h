
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

		using __SuperClass = JSArray<__PtrType>;

		using __PtrArray_InitList = __InitList<__PtrType>;

		using __RefType = __Type&;
		using __ConstPtrRef = const __PtrType&;

		using __PtrArray_CB = __FN_CB<__RefType>;
		using __PtrArray_CB_RetBool = __FN_CB_RetBool<__RefType>;
		using __Data_Pos_CB = const function<bool(__RefType, TD_PosType)>&;

		using __FN_Sort = const function<bool(__RefType ref1, __RefType ref2)>&;

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
		TD_SizeType _add(__ConstPtrRef ptr) override
		{
			return __SuperClass::_add(ptr);
		}

		TD_SizeType _add(__RefType ref)
		{
			return __SuperClass::_add(&ref);
		}
		
		void _unshift(__ConstPtrRef ptr)
		{
			__SuperClass::unshift(ptr);
		}

		void _unshift(__RefType ref)
		{
			__SuperClass::unshift(&ref);
		}

	public:
		bool get(TD_PosType pos, __PtrArray_CB fn) const
		{
			__PtrType ptr = NULL;
			if (!__SuperClass::get(pos, ptr))
			{
				return false;
			}

			if (NULL == ptr)
			{
				return false;
			}

			if (fn)
			{
				fn(*ptr);
			}

			return true;
		}

		bool set(TD_PosType pos, __PtrType ptr)
		{
			return __SuperClass::set(pos, ptr);
		}

		bool set(TD_PosType pos, __RefType data)
		{
			return __SuperClass::set(pos, &data);
		}

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

			for (auto&data : container)
			{
				_add(data);
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

			for (auto&data : container)
			{
				_unshift(data);
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

	public:
		void forEach(__Data_Pos_CB fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			for (TD_PosType pos = startPos; pos < __SuperClass::size(); pos++)
			{
				auto ptr = __SuperClass::at(pos);
				if (NULL == ptr)
				{
					continue;
				}

				if (!fn(*ptr, pos))
				{
					break;
				}

				if (0 < count)
				{
					count--;
					if (0 == count)
					{
						break;
					}
				}
			}
		}

		void forEach(__PtrArray_CB_RetBool fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			forEach([&](__RefType data, TD_PosType pos) {
				return fn(data);
			}, (TD_PosType)startPos);
		}

		int find(__Data_Pos_CB fn, TD_PosType stratPos = 0) const
		{
			if (!fn)
			{
				return -1;
			}

			return __SuperClass::forEach([&](__PtrType ptr, TD_PosType pos) {
				if (NULL == ptr)
				{
					return false;
				}

				return fn(*ptr, pos);
			});
		}

		PtrArray slice(int startPos) const
		{
			PtrArray arr;

			startPos = __SuperClass::_checkPos(startPos);
			if (startPos >= 0)
			{
				forEach([&](__RefType data) {
					arr.push(&data);
				}, (TD_PosType)startPos);
			}

			return arr;
		}

		PtrArray slice(int startPos, int endPos) const
		{
			PtrArray arr;

			startPos = __SuperClass::_checkPos(startPos);
			endPos = __SuperClass::_checkPos(startPos);

			if (startPos >= 0 && endPos >= 0 && startPos <= endPos)
			{
				forEach([&](__RefType data) {
					arr.push(&data);
					return true;
				}, (TD_PosType)startPos, TD_SizeType(endPos - startPos + 1));
			}

			return arr;
		}

		bool front(__PtrArray_CB fn = NULL) const
		{
			return __SuperClass::front([&](__PtrType ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		bool back(__PtrArray_CB fn = NULL) const
		{
			return __SuperClass::back([&](__PtrType ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		bool pop(__PtrArray_CB fn = NULL)
		{
			return __SuperClass::pop([&](__PtrType ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		bool shift(__PtrArray_CB fn = NULL)
		{
			return __SuperClass::shift([&](__PtrType ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		PtrArray& sort(__FN_Sort fn = NULL)
		{
			__SuperClass::sort([&](__PtrType ptr1, __PtrType ptr2) {
				if (NULL != ptr1 && NULL != ptr2)
				{
					return fn(*ptr1, *ptr2);
				}

				return false;
			});
			return *this;
		}

	public:
		template <typename T>
		JSArray<T> map(__FN_CB<__PtrType, T> fn) const
		{
			JSArray<T> arr;

			for (__PtrType ptr : __SuperClass::m_data)
			{
				if (NULL != ptr)
				{
					arr.push(fn(ptr));
				}
			}

			return arr;
		}

		template <typename __Function, typename __RET = decltype(declval<__Function>()(declval<__RefType>()))>
		JSArray<__RET> map(__Function fn) const
		{
			return map<__RET>(fn);
		}

		PtrArray filter(__PtrArray_CB_RetBool fn) const
		{
			PtrArray arr = __SuperClass::filter([&](__PtrType ptr) {
				if (NULL == ptr)
				{
					return false;
				}

				return fn(*ptr);
			});
			
			return arr;
		}

		bool every(__PtrArray_CB_RetBool fn)
		{
			return __SuperClass::every([&](__PtrType ptr) {
				if (NULL == ptr)
				{
					return false;
				}

				return fn(*ptr);
			});
		}

		bool some(__PtrArray_CB_RetBool fn)
		{
			return __SuperClass::some([&](__PtrType ptr) {
				if (NULL == ptr)
				{
					return false;
				}

				return fn(*ptr);
			});
		}

		template<typename T, typename __Function>
		T reduce(const T& stat, __Function fn)
		{
			T ret = stat;
			for (__PtrType ptr : __SuperClass::m_data)
			{
				if (NULL != ptr)
				{
					ret = (T)fn(ret, *ptr);
				}
			}

			return ret;
		}
	};

	template<typename __Type>
	using ConstPtrArray = PtrArray<const __Type>;
}

#endif // __PtrArray_H
