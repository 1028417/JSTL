
#ifndef __JSArray_H
#define __JSArray_H

#include "Container.h"

#include <deque>
#include <map>

#include <memory.h>

namespace NS_JSTL
{
	template<typename __KeyType, typename __ValueType, template<typename...> class __MapType = map> class JSMap;

	template<typename __DataType>
	class JSArray : public  ContainerT<__DataType, deque<__DataType>>
	{
	private:
		using __SuperClass = ContainerT<__DataType, deque<__DataType>>;

		using __JSArray_InitList = __InitList<__DataType>;

		using __ConstDataRef = const __DataType&;

		using __CB_void = __CB_T_void<__ConstDataRef>;
		using __CB_bool = __CB_T_bool<__ConstDataRef>;

		using __CB_ConstDataRef_Pos = __CB_Data_Pos<__ConstDataRef>;

	public:
		static JSArray init(TD_SizeType size, __ConstDataRef data)
		{
			JSArray arr;
			arr.m_data.assign(size, data);
			return arr;
		}

		JSArray()
		{
		}

		template<typename... args>
		explicit JSArray(__ConstDataRef data, const args&... others)
		{
			__SuperClass::add(data, others...);
		}

		explicit JSArray(const JSArray& arr)
			: __SuperClass(arr)
		{
		}

		JSArray(JSArray&& arr)
		{
			__SuperClass::swap(arr);
		}

		explicit JSArray(__JSArray_InitList initList)
			: __SuperClass(initList)
		{
		}

		template<typename T, typename _ITR = decltype(declval<T>().begin())>
		explicit JSArray(const T& container)
			: __SuperClass(container)
		{
		}

		JSArray& operator=(const JSArray& arr)
		{
			__SuperClass::assign(arr);
			return *this;
		}

		JSArray& operator=(JSArray&& arr)
		{
			__SuperClass::swap(arr);
			return *this;
		}

		JSArray& operator=(__JSArray_InitList initList)
		{
			__SuperClass::assign(initList);
			return *this;
		}

		template <typename T>
		JSArray& operator=(const T&t)
		{
			__SuperClass::assign(t);
			return *this;
		}

	protected:
		__DataType* _at(TD_PosType pos)
		{
			if (pos >= __SuperClass::m_data.size())
			{
				return NULL;
			}

			return &__SuperClass::m_data[pos];
		}

		const __DataType* _at(TD_PosType pos) const
		{
			if (pos >= __SuperClass::m_data.size())
			{
				return NULL;
			}

			return &__SuperClass::m_data[pos];
		}

		TD_SizeType _add(__ConstDataRef data) override
		{
			__SuperClass::m_data.push_back(data);
			return __SuperClass::m_data.size();
		}

		TD_SizeType _del(__ConstDataRef data) override
		{
			TD_SizeType uRet = 0;

			auto itr = __SuperClass::m_data.begin();
			while (itr != __SuperClass::m_data.end())
			{
				if (tagTryCompare<__DataType>().compare(*itr, data))
				{
					itr = __SuperClass::m_data.erase(itr);
					uRet++;
				}
				else
				{
					itr++;
				}
			}

			return uRet;
		}

		bool _includes(__ConstDataRef data) const override
		{
			return indexOf(data) >= 0;
		}

		int _checkPos(int pos) const
		{
			auto size = __SuperClass::size();
			if (0 == size)
			{
				return -1;
			}

			if (pos < 0)
			{
				return size + pos;
			}
			else
			{
				if (pos >= (int)size)
				{
					return -1;
				}

				return pos;
			}
		}

	public:
		bool get(TD_PosType pos, __CB_void fn) const
		{
			const __DataType *ptr = _at(pos);
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

		bool get(TD_PosType pos, __DataType& data) const
		{
			const __DataType *ptr = _at(pos);
			if (NULL == ptr)
			{
				return false;
			}

			data = *ptr;

			return true;
		}

		bool set(TD_PosType pos, __ConstDataRef& data)
		{
			__DataType *ptr = _at(pos);
			if (NULL == ptr)
			{
				return false;
			}

			*ptr = data;

			return true;
		}

		int indexOf(__ConstDataRef data) const
		{
			int uIdx = 0;
			for (auto& item : __SuperClass::m_data)
			{
				if (tagTryCompare<__DataType>().compare(item, data))
				{
					return uIdx;
				}
				uIdx++;
			}

			return -1;
		}

		int lastIndexOf(__ConstDataRef data) const
		{
			int uIdx = 1;
			for (auto& item : __SuperClass::m_data)
			{
				if (tagTryCompare<__DataType>().compare(item, data))
				{
					return __SuperClass::m_data.size()-uIdx;
				}
				uIdx++;
			}

			return -1;
		}

		void forEach(__CB_ConstDataRef_Pos fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			__SuperClass::forEach([&](__ConstDataRef data, TD_PosType pos) {
				if (!fn(data, pos))
				{
					return false;
				}

				if (0 < count)
				{
					count--;
					if (0 == count)
					{
						return false;
					}
				}

				return true;
			});
		}

		void forEach(__CB_bool fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			forEach([&](__ConstDataRef data, TD_PosType pos) {
				return fn(data);
			}, (TD_PosType)startPos);
		}

		int find(__CB_ConstDataRef_Pos fn, TD_PosType stratPos = 0) const
		{
			if (!fn)
			{
				return -1;
			}

			int iRet = -1;
			forEach([&](__ConstDataRef data, TD_PosType pos) {
				if (fn(data, pos))
				{
					iRet = pos;
					return false;
				}

				return true;
			});

			return iRet;
		}

		JSArray slice(int startPos) const
		{
			JSArray arr;

			startPos = _checkPos(startPos);
			if (startPos >= 0)
			{
				forEach([&](__ConstDataRef data) {
					arr.push(data);
				}, (TD_PosType)startPos);
			}

			return arr;
		}

		JSArray slice(int startPos, int endPos) const
		{
			JSArray arr;

			startPos = _checkPos(startPos);
			endPos = _checkPos(startPos);

			if (startPos >= 0 && endPos >=0 && startPos <= endPos)
			{
				forEach([&](__ConstDataRef data) {
					arr.push(data);
					return true;
				}, (TD_PosType)startPos, TD_SizeType(endPos-startPos+1));
			}

			return arr;
		}

		template<typename... args>
		TD_SizeType push(__ConstDataRef data, const args&... others)
		{
			return __SuperClass::add(data, others...);
		}

		template<typename T>
		TD_SizeType push(const T& container)
		{
			return __SuperClass::add(container);
		}

		TD_SizeType push(__JSArray_InitList initList)
		{
			return __SuperClass::add(initList);
		}

		template<typename... args>
		JSArray concat(__ConstDataRef data, const args&... others) const
		{
			JSArray arr(*this);
			arr.push(data, others...);
			return arr;
		}

		template<typename T>
		JSArray concat(const T& container) const
		{
			JSArray arr(*this);
			arr.push(container);
			return arr;
		}

		JSArray concat(__JSArray_InitList initList) const
		{
			JSArray arr(*this);
			arr.push(initList);
			return arr;
		}

		bool pop(__CB_void fn=NULL)
		{
			if (__SuperClass::m_data.empty())
			{
				return false;
			}

			if (fn)
			{
				fn(__SuperClass::m_data.back());
			}

			__SuperClass::m_data.pop_back();

			return true;
		}

		bool shift(__CB_void fn=NULL)
		{
			return pop_begin(fn);
		}

		template<typename... args>
		TD_SizeType unshift(__ConstDataRef data, const args&... others)
		{
			(void)__SuperClass::extractDataTypeArgs([&](__ConstDataRef data) {
				__SuperClass::m_data.push_front(data);
				return true;
			}, data, others...);

			return __SuperClass::size();
		}

		template<typename T>
		TD_SizeType unshift(const T& container)
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return __SuperClass::size();
			}

			__SuperClass::m_data.insert(__SuperClass::m_data.begin(), container.begin(), container.end());

			return __SuperClass::m_data.size();
		}

		TD_SizeType unshift(__JSArray_InitList initList)
		{
			return unshift<__JSArray_InitList>(initList);
		}

		template<typename... args>
		JSArray& splice(TD_PosType pos, TD_SizeType nRemove, __ConstDataRef data, const args&... others)
		{
			vector<__DataType> vecData;
			extractDataTypeArgs(vecData, data, others...);
			return splice(pos, nRemove, vecData);
		}

		template<typename T>
		JSArray& splice(TD_PosType pos, TD_SizeType nRemove = 0, const T& container = {})
		{
			if (__SuperClass::checkIsSelf(container))
			{
				return *this;
			}

			auto itr = __SuperClass::m_data.end();
			if (pos < __SuperClass::m_data.size())
			{
				itr = __SuperClass::m_data.begin() + pos;
			}

			while (itr != __SuperClass::m_data.end() && nRemove)
			{
				itr = __SuperClass::m_data.erase(itr);
				nRemove--;
			}

			__SuperClass::m_data.insert(itr, container.begin(), container.end());

			return *this;
		}

		JSArray& splice(TD_PosType pos, TD_SizeType nRemove, __JSArray_InitList initList)
		{
			return splice(pos, nRemove, initList);
		}

		JSArray& sort(__CB_Sort_T<__DataType> fn = NULL)
		{
			std::sort(__SuperClass::m_data.begin(), __SuperClass::m_data.end(), tagTrySort<__DataType>(fn));

			return *this;
		}

		JSArray& reverse()
		{
			std::reverse(__SuperClass::m_data.begin(), __SuperClass::m_data.end());

			return *this;
		}

		string join(const string& strSplitor = ",")
		{
			return __SuperClass::toString(strSplitor);
		}

	public:
		template <typename T>
		JSArray<T> map(__CB_T_RET<__ConstDataRef, T> fn) const
		{
			JSArray<T> arr;

			if (fn)
			{
				for (auto&data : __SuperClass::m_data)
				{
					arr.push(fn(data));
				}
			}

			return arr;
		}

		template <typename __Function, typename __RET = decltype(declval<__Function>()(__DataType()))>
		JSArray<__RET> map(__Function fn) const
		{
			return map<__RET>(fn);
		}

		JSArray filter(__CB_bool fn) const
		{
			JSArray arr;

			for (auto&data : __SuperClass::m_data)
			{
				if (fn(data))
				{
					arr.push(data);
				}
			}

			return arr;
		}

		JSMap<__DataType, TD_SizeType> itemSum() const
		{
			JSMap<__DataType, TD_SizeType> mapItemSum;

			for (auto&data : __SuperClass::m_data)
			{
				mapItemSum[data]++;
			}

			return mapItemSum;
		}

		void sum(JSMap<__DataType, TD_SizeType>& mapItemSum, JSMap<TD_SizeType, JSArray<__DataType>>& mapSumItem) const
		{
			mapItemSum = itemSum();

			for (auto& pr : mapItemSum)
			{
				mapSumItem[pr.second].push(pr.first);
			}
		}

		JSMap<TD_SizeType, JSArray<__DataType>> sumItem() const
		{
			JSMap<__DataType, TD_SizeType> mapItemSum;
			JSMap<TD_SizeType, JSArray<__DataType>> mapSumItem;
			sum(mapItemSum, mapSumItem);
			return mapSumItem;
		}
	};
}

#endif // __JSArray_H
