
#ifndef __Util_H
#define __Util_H

#include <initializer_list>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>

using namespace std;

namespace NS_JSTL
{
	typedef size_t TD_PosType;
	typedef size_t TD_SizeType;
	
	template <typename T>
	using __InitList = const initializer_list<T>&;

	template <typename T, typename __RET = void>
	using __FN_CB = const function<__RET(const T&v)>&;

	template <typename T>
	using __FN_CB_RetBool = __FN_CB<T, bool>;

	template <typename T, typename U>
	struct decay_is_same :
		std::is_same<typename std::decay<T>::type, U>::type
	{};

	template <typename T>
	struct tagTryCompare
	{
		static bool compare(const T&t1, const T&t2)
		{
			return _compare(t1, t2);
		}

		template <typename U>
		static auto _compare(const U&t1, const U&t2) ->decltype(declval<U>() == declval<U>())
		{
			return t1 == t2;
		}

		static bool _compare(...)
		{
			return false;
		}
	};

	template <typename T>
	using __FN_Sort = const function<bool(const T&t1, const T&t2)>&;

	template <typename T>
	struct tagTrySort
	{
		tagTrySort(__FN_Sort<T> fn = NULL)
			: m_fn(fn)
		{
		}

		__FN_Sort<T> m_fn;

		bool operator()(const T&t1, const T&t2)const
		{
			if (m_fn)
			{
				return m_fn(t1, t2);
			}

			return _compare(t1, t2);
		}

		template <typename U>
		static auto _compare(const U&t1, const U&t2) -> decltype(declval<U>() < declval<U>())
		{
			return t1 < t2;
		}

		static bool _compare(...)
		{
			return false;
		}
	};

	template <typename T, typename U>
	struct tagTryLMove
	{
		static void lmove(T&t, const U&u)
		{
			_lmove(&t, &u);
		}

		template <typename X, typename Y>
		static auto _lmove(X*px, const Y*py) -> decltype(declval<X*>()->operator<<(declval<Y>()))
		{
			return *px << *py;
		}

		static bool _lmove(...)
		{
			return false;
		}

		enum { value = std::is_same<decltype(_lmove(declval<T*>(), declval<U*>())), T&>::value };
	};

	template <typename T, typename U = int>
	struct tagLMove
	{
		tagLMove(T&t)
			: m_t(t)
		{
		}

		T& m_t;

		tagLMove& operator<<(const U& u)
		{
			tagTryLMove<T, U>::lmove(m_t, u);
			return *this;;
		}

		template <typename V>
		tagLMove& operator<<(const V&v)
		{
			tagLMove<T, V>(m_t) << v;
			return *this;
		}
	};

	using tagSSTryLMove = tagLMove<stringstream>;

	template<typename __DataType>
	class tagDynamicArgsExtractor
	{
	public:
		using FN_ExtractCB = function<bool(__DataType&v)>;

		template<typename... args>
		static bool extract(const FN_ExtractCB& cb, __DataType&v, args&... others)
		{
			if (!cb)
			{
				return false;
			}

			if (!extract(cb, v))
			{
				return false;
			}

			if (sizeof...(others))
			{
				if (!extract(cb, others...))
				{
					return false;
				}
			}

			return true;
		}

		static bool extract(const FN_ExtractCB& cb, __DataType&v)
		{
			return cb(v);
		}
	};

	template <typename T, typename _RetType, typename _ITR = decltype(declval<T>().begin())>
	_RetType checkContainer();
}

#endif // __Util_H
