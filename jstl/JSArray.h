
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

		using __JSArray_CB = __FN_CB<__DataType>;
		using __JSArray_CB_RetBool = __FN_CB_RetBool<__DataType>;

		using __ConstDataRef = const __DataType&;

		using __Data_Pos_CB = const function<bool(__ConstDataRef, TD_PosType)>&;

	public:
		static JSArray init(TD_SizeType size, __ConstDataRef data)
		{
			JSArray arr;
			arr._init(size, data);
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
		void _init(TD_SizeType size, __ConstDataRef data)
		{
			JSArray arr;
			__SuperClass::m_data.assign(size, data);
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
		__DataType& operator[](TD_PosType pos)
		{
			return __SuperClass::m_data[pos];
		}
		__ConstDataRef operator[](TD_PosType pos) const
		{
			return __SuperClass::m_data[pos];
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

		bool get(TD_PosType pos, __JSArray_CB fn) const
		{
			if (pos >= __SuperClass::m_data.size())
			{
				return false;
			}

			if (fn)
			{
				fn(__SuperClass::m_data[pos]);
			}

			return true;
		}

		void forEach(__Data_Pos_CB fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			for (TD_PosType pos = startPos; pos < __SuperClass::m_data.size(); pos++)
			{
				if (!fn(__SuperClass::m_data[pos], pos))
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

		void forEach(__JSArray_CB_RetBool fn, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!fn)
			{
				return;
			}

			forEach([&](__ConstDataRef data, TD_PosType pos) {
				return fn(data);
			}, (TD_PosType)startPos);
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

		template <typename __Function>
		int find(__Function fn, TD_PosType stratPos = 0) const
		{
			for (TD_PosType pos = stratPos; pos < __SuperClass::m_data.size(); pos++)
			{
				if (fn(pos, __SuperClass::m_data[pos]))
				{
					return pos;
				}

				pos++;
			}

			return -1;
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

		bool pop(__JSArray_CB fn=NULL)
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

		bool shift(__JSArray_CB fn=NULL)
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

		JSArray& sort(__FN_Sort<__DataType> fn = NULL)
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
		JSArray filter(__JSArray_CB_RetBool fn) const
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
