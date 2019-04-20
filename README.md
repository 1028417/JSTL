# **SSTL项目介绍**
## **创作背景**
C++标准模块库用起来很不灵活，尤其和脚本语言对比起来更加显得耗费大量代码。  

最初参考Nodejs，在STL基础上利用C++11特性封装为JSTL，后来改名为超级模板库。

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
* insert、set、get方法用于插入、读写；
* keys、values方法用于返回或遍历键/值；
* filter、filterKey、filterValue方法用于过滤；
* map、mapKey、mapValue方法用于映射；
* every、everyKey、everyValue、any、anyKey、anyValue方法用于整体判断。

### 关联链表：ArrList
* 内置vector记录链表节点指针，可随机访问内部的元素；
* add、get、set方法用于追加、读写；
* qsort方法用于快速排序。

### pair链表：PairList
* 内部存储泛型pair对象，firsts、seconds方法用于返回或遍历分量；
* add、addPair方法用于追加；
* get、getFirst、getSecond、set、setFirst、setSecond方法用于读写；
* filter、filterFirst、filterSecond方法用于过滤；
* map、mapKey、mapValue方法用于映射；
* every、everyFirst、everySecond、any、anyFirst、anySecond方法用于整体判断；
* qsort、qsortFirst、qsortSecond方法用于快速排序。

### 指针链表：PtrList
* 内部存储指针，各接口提供dynamic_cast安全转换和基于引用的安全访问；
* qsort方法用于快速排序。

## 使用方法
包含jstl.h一个头文件即可，test.cpp中有些简单的用例，VC、GCC、Qt(MinGW)编译通过。
