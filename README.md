# **SSTL项目介绍**
## **背景**
C++标准模块库用起来很不灵活，尤其和脚本语言对比起来更加显得耗费大量代码。  

最初参考Nodejs，在STL基础上利用C++11特性封装为JSTL,后来改名为超级模板库。

## **模板类**
### 数组：SArray/SVector
* 所有模板的构造函数、赋值操作符、assign、add等方法都支持传入多个元素或容器或初始列表； 
圆括号操作符提供按范围遍历；
* qsort方法实现快速排序法；
* 仿nodejs实现slice、splice、concat、map、filter等方法；
* 仿python实现every、any方法。

### 链表：SList
简单封装，为了配套。

### 集合：SSet、SHashSet
基类SSetT还可以代入multiset、unordered_multiset实现重复元素共存的模板。

### 映射：SMap、SHashMap
* 基类SMapT还可以代入multimap、unordered_multimap实现重复key共存的模板；
* keys、values用于键/值遍历；
* filter、filterKey、filterValue用于过滤；
* mapKey、mapValue用于映射；
* everyKey、everyValue、anyKey、anyValue整体判断。

### 随机链表：ArrList

### pair链表：PairList

### 指针链表：PtrList

## 用法
包含jstl.h一个头文件即可，test.cpp中有些简单的用例。
