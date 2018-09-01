
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

		using __CB_RefType_void = __CB_T_void<__RefType>;
		using __CB_RefType_bool = __CB_T_bool<__RefType>;

		using __CB_RefType_Pos = __CB_Data_Pos<__RefType>;

		using __CB_Sort_RefType = __CB_Sort_T<__Type>;

	public:
		PtrArray()
		{
		}

		template<typename... args>
		explicit PtrArray(__ConstPtrRef ptr, const args&... others)
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
		bool get(TD_PosType pos, __CB_RefType_void fn) const
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

		bool set(TD_PosType pos, __ConstPtrRef ptr)
		{
			return __SuperClass::set(pos, ptr);
		}

		bool set(TD_PosType pos, __RefType data)
		{
			return __SuperClass::set(pos, &data);
		}

		template<typename... args>
		PtrArray& assign(__ConstPtrRef ptr, const args&... others)
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
		TD_SizeType push(__ConstPtrRef ptr, const args&... others)
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
		PtrArray concat(__ConstPtrRef ptr, const args&... others) const
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
		TD_SizeType unshift(__ConstPtrRef ptr, const args&... others)
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
		PtrArray& splice(TD_PosType pos, TD_SizeType nRemove, __ConstPtrRef v, const args&... others)
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
		void forEach(__CB_RefType_Pos fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
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

		void forEach(__CB_RefType_bool fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			forEach([&](__RefType data, TD_PosType pos) {
				return fn(data);
			}, (TD_PosType)startPos);
		}

		int find(__CB_RefType_Pos fn, TD_PosType stratPos = 0) const
		{
			if (!fn)
			{
				return -1;
			}

			return __SuperClass::forEach([&](__ConstPtrRef ptr, TD_PosType pos) {
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

		bool front(__CB_RefType_void fn = NULL) const
		{
			return __SuperClass::front([&](__ConstPtrRef ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		bool back(__CB_RefType_void fn = NULL) const
		{
			return __SuperClass::back([&](__ConstPtrRef ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		bool pop(__CB_RefType_void fn = NULL)
		{
			return __SuperClass::pop([&](__ConstPtrRef ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		bool shift(__CB_RefType_void fn = NULL)
		{
			return __SuperClass::shift([&](__ConstPtrRef ptr) {
				if (NULL != ptr)
				{
					if (fn)
					{
						fn(ptr);
					}
				}
			});
		}

		PtrArray& sort(__CB_Sort_RefType fn)
		{
            if (fn)
            {
                __SuperClass::sort([&](__ConstPtrRef ptr1, __ConstPtrRef ptr2) {
                    if (NULL != ptr1 && NULL != ptr2)
                    {
                        return fn(*ptr1, *ptr2);
                    }

                    return false;
                });
            }

			return *this;
		}

	public:
		template <typename T>
		JSArray<T> map(__CB_T_RET<__RefType, T> fn) const
		{
			JSArray<T> arr;

			forEach([&](__RefType ref) {
				arr.push(fn(ref));
				return true;
			});

			return arr;
		}

		template <typename __Function, typename __RET = decltype(declval<__Function>()(declval<__RefType>()))>
		JSArray<__RET> map(__Function fn) const
		{
			return map<__RET>(fn);
		}

		PtrArray filter(__CB_RefType_bool fn) const
		{
			PtrArray arr = __SuperClass::filter([&](__ConstPtrRef ptr) {
				if (NULL == ptr)
				{
					return false;
				}

				return fn(*ptr);
			});

			return arr;
		}

		bool every(__CB_RefType_bool fn)
		{
			return __SuperClass::every([&](__ConstPtrRef ptr) {
				if (NULL == ptr)
				{
					return false;
				}

				return fn(*ptr);
			});
		}

		bool some(__CB_RefType_bool fn)
		{
			return __SuperClass::some([&](__ConstPtrRef ptr) {
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
			forEach([&](__RefType ref) {
				ret = (T)fn(ret, ref);
			});

			return ret;
		}
	};

	template<typename __Type>
	using ConstPtrArray = PtrArray<const __Type>;
}

#endif // __PtrArray_H
