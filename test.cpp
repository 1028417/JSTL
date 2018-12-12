
#include "sstl/sstl.h"
using namespace NS_SSTL;

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

bool enumCardPattern(ECardPattern eCardPattern, size_t uFlag, const SMap<TD_CardNum, size_t>& mapCardSum, TD_CardNum uBaseCardNum, size_t uContinuityCount, SArray<tagCardPattern>& retCardPattern)
{
	bool bRet = false;

	SArray<TD_CardNum> arrCardExpect = mapCardSum.filter([&](const TD_CardNum&uCardNum, const size_t& uSum) {
		return uSum >= uFlag && uCardNum>= uBaseCardNum;
	}).keys();

	arrCardExpect([&](TD_PosType pos, const TD_CardNum&uCardNum) {
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
	
	SMap<TD_CardNum, size_t> mapCardSum;

	SArray<tagCardPattern> retCardPattern;
	return enumCardPattern(eCardPattern, uCardPatternFlag, mapCardSum, uBaseCardNum, uContinuityCount, retCardPattern);
}


SMap<int, set<int>> sss()
{
	return SMap<int, set<int>>();
}

int main()
{
	SMap<int, set<int>> jjhh;
	jjhh = jjhh;

	struct tag {};
	SArray<tag> qqta{ {},{},{},{} };
	auto idof = qqta.indexOf({});
	qqta.qsort();
	auto strsk = qqta.toString();
	
	SArray<int> qq;
	qq= { 12,2,3 };
	qq = 1;
	auto aaaa = qq.indexOf({2});
	qq.qsort();
	auto aaaaaaa = qq.toString();
	qq.addFront({ 1 });
	qq.addFront(1);
	qq.addFront(1,2);
	
	//qq = ll;
	auto idf = qq.indexOf(2);

	SMap<int, int> masaa{ { 21,2 },{ 1,2 },{ 1,2 } };
	auto hbfdjv = masaa.toString();

	SArray<int> arr(qq);
	arr=qq;
	arr.add({ 66,44,19,2,2,3,5,5,6,7,8 });
	qq.add( { 12,2,3 });
	SArray<int> arr2;
	arr2 = arr.map([](float i) {return i * 2; });
	auto ff = arr.reduce([](const float& a1, const int& a2) {return a1+a2; });

	SArray<int> arr3 = arr2.slice(6, 3);
	arr3 = arr3;

	SMap<int, int> ooo({1}, [](int key) {
		return key*2;
	});

	map<int, int> tm;
	SMap<int,int> mmm(tm);
	mmm = { {1,1} };
	mmm = tm;
	mmm.keys();
	
	mmm.del(1, 2);
	mmm.del(11);
	mmm.del({ 1, 2 });
	mmm.del(arr3);

	SSet<int> set ;
	set.add({ 2,3,4 });
	set.add( 1,2,3 );

	SSet<int> set2 ( {1});
	set2 = arr2;
	set2.add({3,4,5});

	SMap<int, int> m{ { 21,2 },{ 1,2 },{ 1,2 } };
	m.set(1, 1);

	enumCardPattern(CP_None);

	SMap<int, int> ts1;
	ts1.assign( { { 1, 1 }, { 32,1 }, { 3,1 } });

	SArray<int> ts2(7,7.9, 4);
	ts2.add({1,2,3});
	ts2 = {1,2,3};

	ts2.getFront([](int&) {});
	((const SArray<int>&)ts2).getFront([](const int&) {});

	ts2([](int i) {
		return true;
	});

	ts2.qsort();
	ts2.concat(9);
	ts2.concat({ 9 });
	ts2.concat(9, 9);
	ts2.concat(ts2);
	
	ts2.del(9, 9);
	ts2.del(9);
	ts2.del({ 9, 9 });
	ts2.del(ts2);

	SArray<int*> pils;
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

	PtrArray111.addFront(uy, uy);
	PtrArray111.addFront(&uy, &uy);
	PtrArray111.addFront({ &uy });
	PtrArray111.addFront(PtrArray111);

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

	ConstPtrArray111.addFront(uy);
	ConstPtrArray111.addFront(&uy);
	ConstPtrArray111.addFront({ &uy });
	ConstPtrArray111.addFront(PtrArray111);
	ConstPtrArray111.addFront(ConstPtrArray111);

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

	SArray<int> jahss(5, 5, 5, 5);
	SSet<int> afrdg(5, 5, 5, 4);
	bool bbb = afrdg.includes(5);

	int t= afrdg.reduce([](int a, int b) {
		return a + b;
	});

	SSet<int> sstt({1,2});
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

	popopo.addFront((int*)nullptr);
	popopo.addFront({ nullptr, nullptr, nullptr });
	popopo.addFront(*(int*)nullptr);
	popopo.addFront(jahss);
	popopo.addFront(zqq161);

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
	
	SHashSet<int> oiuy{4,4,5,5,3,3};
	oiuy-vector<int>();
	oiuy - oiuy;
	SHashSet<int> qafjdjsd = oiuy & qq;
	
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

	SArray<int> gggg(1, 3, 5, 7, 9);
	bool bin = gggg.includes(1, 3, 5, 7, 9);
	bin = gggg.includes(1, 3, 5, 7, 9, 6);
	
	

	SMap<int, int> mgmg;
	bin = mgmg.includes(1, 3, 5, 7, 9);
	bin = mgmg.includes(1, 5, 9);

	return 0;
}
