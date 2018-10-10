
#ifndef __define_h
#define __define_h

#include <initializer_list>

#include <functional>

using namespace std;

typedef size_t TD_PosType;
typedef size_t TD_SizeType;

template <typename T>
using InitList_T = const initializer_list<T>&;

template <typename T, typename RET>
using CB_T_Ret = const function<RET(const T&v)>&;

template <typename T>
using CB_T_void = CB_T_Ret<T, void>;

template <typename T>
using CB_T_bool = CB_T_Ret<T, bool>;

template <typename T>
using CB_T_Pos = const function<bool(T, TD_PosType)>&;

#define __SuperType(T) typename __Super::T
#define __UsingSuperType(T) using T = __SuperType(T)

#endif // __define_h
