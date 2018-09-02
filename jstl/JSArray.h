
#ifndef __JSArray_H
#define __JSArray_H

#include "Container.h"

#include <deque>

#include "JSMap.h"

namespace NS_JSTL
{
	template<typename __DataType>
	class JSArray : public  ContainerT<__DataType, deque<__DataType>>
	{
	protected:
		using __ContainerType = deque<__DataType>;
		using __SuperClass = ContainerT<__DataType, __ContainerType>;

		using __JSArray_InitList = __InitList<__DataType>;

		using __DataRef = __DataType&;
		using __ConstDataRef = const __DataType&;

		using __CB_Ref_void = __CB_T_void<__DataRef>;
		using __CB_Ref_bool = __CB_T_bool<__DataRef>;

		using __CB_ConstRef_void = __CB_T_void<__ConstDataRef>;
		using __CB_ConstRef_bool = __CB_T_bool<__ConstDataRef>;

		using __CB_Ref_Pos = __CB_Data_Pos<__DataRef>;
		using __CB_ConstRef_Pos = __CB_Data_Pos<__ConstDataRef>;

	protected:
		__ContainerType& _data()
		{
			return __SuperClass::m_data;
		}

		const __ContainerType& _data() const
		{
			return __SuperClass::m_data;
		}

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
		TD_SizeType _add(__ConstDataRef data) override
		{
			_data().push_back(data);
			return _data().size();
		}

		TD_SizeType _del(__ConstDataRef data) override
		{
			TD_SizeType uRet = 0;

			auto itr = _data().begin();
			while (itr != _data().end())
			{
				if (tagTryCompare<__DataType>().compare(*itr, data))
				{
					itr = _data().erase(itr);
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
				return (int)size + pos;
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
		bool get(TD_PosType pos, __CB_Ref_void cb)
		{
			if (pos >= _data().size())
			{
				return false;
			}
			
			if (cb)
			{
				cb(_data()[pos]);
			}

			return true;
		}

		bool get(TD_PosType pos, __CB_ConstRef_void cb) const
		{
			if (pos >= _data().size())
			{
				return false;
			}

			if (cb)
			{
				cb(_data()[pos]);
			}

			return true;
		}

		bool get(TD_PosType pos, __DataRef data)
		{
			return get([&](__DataRef _data) {
				data = _data;
			});
		}
		
		bool set(TD_PosType pos, __ConstDataRef& data)
		{
			return get([&](__DataRef _data) {
				_data = data;
			});
		}

		int indexOf(__ConstDataRef data) const
		{
			int uIdx = 0;
			for (auto& item : _data())
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
			for (auto& item : _data())
			{
				if (tagTryCompare<__DataType>().compare(item, data))
				{
					return _data().size()-uIdx;
				}
				uIdx++;
			}

			return -1;
		}

		TD_SizeType forEach(__CB_ConstRef_Pos cb, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!cb)
			{
				return 0;
			}

			return __SuperClass::forEach([&](__ConstDataRef data, TD_PosType pos) {
				if (!cb(data, pos))
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

		TD_SizeType forEach(__CB_Ref_Pos cb, TD_PosType startPos = 0, TD_SizeType count = 0)
		{
			if (!cb)
			{
				return 0;
			}

			return this->forEach([&](__ConstDataRef data, TD_PosType pos) {
				return cb((__DataRef)data, pos);
			}, startPos, count);
		}

		TD_SizeType forEach(__CB_ConstRef_bool cb, TD_PosType startPos = 0, TD_SizeType count = 0) const
		{
			if (!cb)
			{
				return 0;
			}

			return this->forEach([&](__ConstDataRef data, TD_PosType pos) {
				return cb(data);
			}, startPos, count);
		}

		TD_SizeType forEach(__CB_Ref_bool cb, TD_PosType startPos = 0, TD_SizeType count = 0)
		{
			if (!cb)
			{
				return 0;
			}

			return this->forEach([&](__ConstDataRef data, TD_PosType pos) {
				return cb((__DataRef)data);
			}, startPos, count);
		}

		int find(__CB_ConstRef_Pos cb, TD_PosType stratPos = 0) const
		{
			if (!cb)
			{
				return -1;
			}

			int iRet = -1;
			forEach([&](__ConstDataRef data, TD_PosType pos) {
				if (cb(data, pos))
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
			endPos = _checkPos(endPos);

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

		bool pop(__CB_ConstRef_void cb = NULL)
		{
			if (_data().empty())
			{
				return false;
			}

			if (cb)
			{
				cb(_data().back());
			}

			_data().pop_back();

			return true;
		}

		bool pop(__DataRef data)
		{
			if (_data().empty())
			{
				return false;
			}

			data = _data().back();
			_data().pop_back();

			return true;
		}

		bool shift(__CB_ConstRef_void cb = NULL)
		{
			if (_data().empty())
			{
				return false;
			}

			if (cb)
			{
				cb(_data().front());
			}

			_data().pop_front();

			return true;
		}

		bool shift(__DataRef data)
		{
			if (_data().empty())
			{
				return false;
			}

			data = _data().front();
			_data().pop_front();

			return true;
		}

		template<typename... args>
		TD_SizeType unshift(__ConstDataRef data, const args&... others)
		{
			(void)__SuperClass::extractDataTypeArgs([&](__ConstDataRef data) {
				_data().push_front(data);
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

			_data().insert(_data().begin(), container.begin(), container.end());

			return _data().size();
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

			auto itr = _data().end();
			if (pos < _data().size())
			{
				itr = _data().begin() + pos;
			}

			while (itr != _data().end() && nRemove)
			{
				itr = _data().erase(itr);
				nRemove--;
			}

			_data().insert(itr, container.begin(), container.end());

			return *this;
		}

		JSArray& splice(TD_PosType pos, TD_SizeType nRemove, __JSArray_InitList initList)
		{
			return splice(pos, nRemove, initList);
		}

		JSArray& sort(__CB_Sort_T<__DataType> cb = NULL)
		{
			std::sort(_data().begin(), _data().end(), tagTrySort<__DataType>(cb));

			return *this;
		}

		JSArray& reverse()
		{
			std::reverse(_data().begin(), _data().end());

			return *this;
		}

		string join(const string& strSplitor = ",") const
		{
			return __SuperClass::toString(strSplitor);
		}

	public:
		template <typename T>
		JSArray<T> map(__CB_T_RET<__ConstDataRef, T> cb) const
		{
			JSArray<T> arr;

			if (cb)
			{
				for (auto&data : _data())
				{
					arr.push(cb(data));
				}
			}

			return arr;
		}

		template <typename CB, typename __RET = decltype(declval<CB>()(__DataType()))>
		JSArray<__RET> map(const CB& cb) const
		{
			return map<__RET>(cb);
		}

		JSArray filter(__CB_ConstRef_bool cb) const
		{
			JSArray arr;

			if (cb)
			{
				for (auto&data : _data())
				{
					if (cb(data))
					{
						arr.push(data);
					}
				}
			}

			return arr;
		}

		JSMap<__DataType, TD_SizeType> itemSum() const
		{
			JSMap<__DataType, TD_SizeType> mapItemSum;

			for (auto&data : _data())
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
