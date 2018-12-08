
#include "jstl/jstl.h"
using namespace NS_JSTL;

typedef size_t TD_CardNum;

enum ECardPattern
{
	CP_None
};

struct tagCardPattern
{
	tagCardPattern()
	{
	}

	tagCardPattern(ECardPattern t_eCardPattern, TD_CardNum t_uBaseCardNum, size_t t_uContinuityCount)
		: eCardPattern(t_eCardPattern)
		, uBaseCardNum(t_uBaseCardNum)
		, uContinuityCount(t_uContinuityCount)
	{
	}

	ECardPattern eCardPattern = CP_None;
	TD_CardNum uBaseCardNum = 0;
	size_t uContinuityCount;

	bool operator==(const tagCardPattern& oher) const
	{
		return eCardPattern == oher.eCardPattern && uBaseCardNum == oher.uBaseCardNum && uContinuityCount == oher.uContinuityCount;
	}

	bool operator>(const tagCardPattern& other)
	{
		return eCardPattern == other.eCardPattern && uContinuityCount == other.uContinuityCount && uBaseCardNum > other.uBaseCardNum;
	}
};

static map<ECardPattern, size_t> g_mapCardPattern{};

bool enumCardPattern(ECardPattern eCardPattern, size_t uFlag, const JSMap<TD_CardNum, size_t>& mapCardSum, TD_CardNum uBaseCardNum, size_t uContinuityCount, JSArray<tagCardPattern>& retCardPattern)
{
	bool bRet = false;

	JSArray<TD_CardNum> arrCardExpect = mapCardSum.filter([&](const TD_CardNum&uCardNum, const size_t& uSum) {
		return uSum >= uFlag && uCardNum>= uBaseCardNum;
	}).keys();

	arrCardExpect.forEach([&](TD_PosType pos, const TD_CardNum&uCardNum) {
		if (1 == uContinuityCount)
		{
			retCardPattern.add(tagCardPattern(eCardPattern, uCardNum, 1));
			bRet = true;
		}
		else
		{
			if (!arrCardExpect.get(pos + uContinuityCount - 1, [&](const TD_CardNum&uCardNum) {
				if (uCardNum - uCardNum + 1 == uContinuityCount)
				{
					retCardPattern.add(tagCardPattern(eCardPattern, uCardNum, 1));
					bRet = true;
				}
			}))
			{
				return false;
			}
		}

		return true;
	});

	return bRet;
}

bool enumCardPattern(ECardPattern eCardPattern, TD_CardNum uBaseCardNum = 0, size_t uContinuityCount=0)
{
	auto itr = g_mapCardPattern.find(eCardPattern);
	if (itr == g_mapCardPattern.end())
	{
		return false;
	}
	size_t uCardPatternFlag = itr->second;
	
	JSMap<TD_CardNum, size_t> mapCardSum;

	JSArray<tagCardPattern> retCardPattern;
	return enumCardPattern(eCardPattern, uCardPatternFlag, mapCardSum, uBaseCardNum, uContinuityCount, retCardPattern);
}


JSMap<int, set<int>> sss()
{
	return JSMap<int, set<int>>();
}

int main()
{
	JSMap<int, set<int>> jjhh;
	jjhh = jjhh;

	struct tag {};
	JSArray<tag> qqta{ {},{},{},{} };
	auto idof = qqta.indexOf({});
	qqta.qsort();
	auto strsk = qqta.toString();
	
	JSArray<int> qq;
	qq= { 12,2,3 };
	qq = 1;
	auto aaaa = qq.indexOf({2});
	qq.qsort();
	auto aaaaaaa = qq.toString();
	qq.unshift({ 1 });
	qq.unshift(1);
	qq.unshift(1,2);
	
	//qq = ll;
	auto idf = qq.indexOf(2);

	JSMap<int, int> masaa{ { 21,2 },{ 1,2 },{ 1,2 } };
	auto hbfdjv = masaa.toString();

	JSArray<int> arr(qq);
	arr=qq;
	arr.add({ 66,44,19,2,2,3,5,5,6,7,8 });
	qq.add( { 12,2,3 });
	JSArray<int> arr2;
	arr2 = arr.map([](float i) {return i * 2; });
	auto ff = arr.reduce([](const float& a1, const int& a2) {return a1+a2; });

	JSArray<int> arr3 = arr2.slice(6, 3);
	arr3 = arr3;

	JSMap<int, int> ooo({1}, [](int key) {
		return key*2;
	});

	map<int, int> tm;
	JSMap<int,int> mmm(tm);
	mmm = { {1,1} };
	mmm = tm;
	mmm.keys();
	mmm.getInner(1);
	mmm.getInner({ 1, 2 });
	mmm.getInner(arr3);
	mmm.getOuter(1, 2);
	mmm.getOuter(11);
	mmm.getOuter({ 1, 2 });
	mmm.getOuter(arr3);

	mmm.del(1, 2);
	mmm.del(11);
	mmm.del({ 1, 2 });
	mmm.del(arr3);

	JSSet<int> set ;
	set.add({ 2,3,4 });
	set.add( 1,2,3 );

	JSSet<int> set2 ( {1});
	set2 = arr2;
	set2.add({3,4,5});

	JSMap<int, int> m{ { 21,2 },{ 1,2 },{ 1,2 } };
	m.set(1, 1);

	enumCardPattern(CP_None);

	JSMap<int, int> ts1;
	ts1.assign( { { 1, 1 }, { 32,1 }, { 3,1 } });

	JSArray<int> ts2(7,7.9, 4);
	ts2.add({1,2,3});
	ts2 = {1,2,3};

	ts2.getFront([](int&) {});
	((const JSArray<int>&)ts2).getFront([](const int&) {});

	ts2.forEach([](int i) {
		return true;
	});

	ts2.qsort();
	ts2.concat(9);
	ts2.concat({ 9 });
	ts2.concat(9, 9);
	ts2.concat(ts2);
	ts2.getInner(9);
	ts2.getInner(9, 9);
	ts2.getInner({ 9, 9 });
	ts2.getInner(ts2);
	ts2.getOuter(9, 9);
	ts2.getOuter(9);
	ts2.getOuter({ 9, 9 });
	ts2.getOuter(ts2);

	ts2.del(9, 9);
	ts2.del(9);
	ts2.del({ 9, 9 });
	ts2.del(ts2);

	JSArray<int*> pils;
	int uy = 1;
	PtrArray<int> PtrArray0001(uy, uy);
	PtrArray<int> PtrArray111(&uy, &uy);
	

	int p1111 = 1;
	//PtrArray111.del(p1111, p1111);

	PtrArray111.assign(uy, uy);
	PtrArray111.assign(&uy, &uy);
	PtrArray111.add(uy, uy);
	PtrArray111.add(&uy, &uy);
	PtrArray111 = PtrArray111;
	PtrArray111.add({ &uy });
	PtrArray111.add(PtrArray111);

	PtrArray111.unshift(uy, uy);
	PtrArray111.unshift(&uy, &uy);
	PtrArray111.unshift({ &uy });
	PtrArray111.unshift(PtrArray111);

	PtrArray111.concat(uy, uy);
	PtrArray111.concat(&uy, &uy);
	PtrArray111.concat({ &uy });
	PtrArray111.concat(PtrArray111);

	ConstPtrArray<int> ConstPtrArray111(&(const int&)uy);
	ConstPtrArray111.add(uy);
	ConstPtrArray111.add(&uy);
	ConstPtrArray111.add({ &uy });
	ConstPtrArray111.add(PtrArray111);
	ConstPtrArray111.add(ConstPtrArray111);

	ConstPtrArray111.unshift(uy);
	ConstPtrArray111.unshift(&uy);
	ConstPtrArray111.unshift({ &uy });
	ConstPtrArray111.unshift(PtrArray111);
	ConstPtrArray111.unshift(ConstPtrArray111);

	ConstPtrArray111.concat(uy);
	ConstPtrArray111.concat(&uy);
	ConstPtrArray111.concat({ &uy });
	ConstPtrArray111.concat(PtrArray111);
	ConstPtrArray111.concat(ConstPtrArray111);


	ConstPtrArray111.add(PtrArray111);
	ConstPtrArray111.concat(PtrArray111);

	ConstPtrArray<int> ConstPtrArray222((const ConstPtrArray<int>&)ConstPtrArray111);
	ConstPtrArray222 = (const ConstPtrArray<int>&)ConstPtrArray111;
	ConstPtrArray222.add(ConstPtrArray111);
	ConstPtrArray222.concat(ConstPtrArray111);
	ConstPtrArray222.add(nullptr);

	bool b = ConstPtrArray222.includes(&uy);
	ConstPtrArray222.del((int*)nullptr, (int*)nullptr, (int*)nullptr);
	//ConstPtrArray222.del(NULL, NULL, NULL);

	ConstPtrArray222 = 5;

	JSArray<int> jahss(5, 5, 5, 5);
	JSSet<int> afrdg(5, 5, 5, 4);
	bool bbb = afrdg.includes(5);

	int t= afrdg.reduce([](int a, int b) {
		return a + b;
	});

	JSSet<int> sstt({1,2});
	sstt -= afrdg;
	sstt += {5};

	PtrArray<int> zqq11((int*)nullptr);
	PtrArray<int> zqq211({nullptr, nullptr, nullptr});
	PtrArray<int> zqq311(*(int*)nullptr);
	PtrArray<int> zqq161(jahss);
	PtrArray<int> zqq711(zqq161);
	
	int yyy = 7;
	list<int*> l;
	PtrArray<int> popopo(l);
	popopo = yyy;
	popopo += (int*)nullptr;
	popopo += {nullptr, nullptr, nullptr};
	popopo += *(int*)nullptr;
	popopo += jahss;
	popopo += popopo;
	popopo = l;
	popopo = ((int*)nullptr);
	popopo = { nullptr, nullptr, nullptr };
	popopo = (*(int*)nullptr);
	popopo = jahss;
	popopo = zqq161;

	popopo.unshift((int*)nullptr);
	popopo.unshift({ nullptr, nullptr, nullptr });
	popopo.unshift(*(int*)nullptr);
	popopo.unshift(jahss);
	popopo.unshift(zqq161);

	popopo.assign((int*)nullptr);
	popopo.assign({ nullptr, nullptr, nullptr });
	popopo.assign(*(int*)nullptr);
	popopo.assign(jahss);
	popopo.assign(zqq161);
	

	popopo.add( (int*)nullptr);
	popopo.add({nullptr, nullptr, nullptr});
	popopo.add(*(int*)nullptr);
	popopo.add(jahss);

	popopo -= (int*)nullptr;
	popopo -= {nullptr, nullptr, nullptr};
	popopo -= popopo;

	popopo.del( (int*)nullptr);
	popopo.del( {nullptr, nullptr, nullptr});
	popopo.del( popopo);

	ConstPtrArray<int> ccccc;
	ccccc.add(ccccc);
	ccccc += ccccc;
	ccccc += sstt;
	
	JSUnsortSet<int> oiuy{4,4,5,5,3,3};
	oiuy-vector<int>();
	oiuy - oiuy;
	JSUnsortSet<int> qafjdjsd = oiuy&qq;
	
	float *pf=0;
	list<int> lala;
	ptrlist<int> pll;
	pll.add(lala);

	PtrArray<int> ttrt;
	ttrt([&](int&) {});
	ttrt([&](int&) {return true; });

	int iii;

	class A
	{
	public:A() {}
		   virtual ~A(){}
	};
	class B : public A
	{
	public:B() {}
	};

	A qaq;
	PtrArray<B> okok;// (qaq, qaq, qaq);
	okok.add(&qaq);

	JSArray<int> gggg(1, 3, 5, 7, 9);
	bool bin = gggg.includes(1, 3, 5, 7, 9);
	bin = gggg.includes(1, 3, 5, 7, 9, 6);
	
	

	JSMap<int, int> mgmg;
	bin = mgmg.includes(1, 3, 5, 7, 9);
	bin = mgmg.includes(1, 5, 9);

	return 0;
}
