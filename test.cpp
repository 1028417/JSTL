
#include "jstl/JSArray.h"
#include "jstl/PtrArray.h"
#include "jstl/JSSet.h"
#include "jstl/JSMap.h"
using namespace NS_JSTL;

#include <list>

typedef unsigned int UINT;

typedef UINT TD_CardNum;

enum ECardPattern
{
	CP_None
};

struct tagCardPattern
{
	tagCardPattern()
	{
	}

	tagCardPattern(ECardPattern t_eCardPattern, TD_CardNum t_uBaseCardNum, UINT t_uContinuityCount)
		: eCardPattern(t_eCardPattern)
		, uBaseCardNum(t_uBaseCardNum)
		, uContinuityCount(t_uContinuityCount)
	{
	}

	ECardPattern eCardPattern = CP_None;
	TD_CardNum uBaseCardNum = 0;
	UINT uContinuityCount;

	bool operator==(const tagCardPattern& oher) const
	{
		return eCardPattern == oher.eCardPattern && uBaseCardNum == oher.uBaseCardNum && uContinuityCount == oher.uContinuityCount;
	}

	bool operator>(const tagCardPattern& other)
	{
		return eCardPattern == other.eCardPattern && uContinuityCount == other.uContinuityCount && uBaseCardNum > other.uBaseCardNum;
	}
};

static map<ECardPattern, UINT> g_mapCardPattern{};

bool enumCardPattern(ECardPattern eCardPattern, UINT uFlag, const JSMap<TD_CardNum, UINT>& mapCardSum, TD_CardNum uBaseCardNum, UINT uContinuityCount, JSArray<tagCardPattern>& retCardPattern)
{
	bool bRet = false;

	JSArray<TD_CardNum> arrCardExpect = mapCardSum.filter([&](const TD_CardNum&uCardNum, const UINT& uSum) {
		return uSum >= uFlag && uCardNum>= uBaseCardNum;
	}).keys();

	arrCardExpect.forEach([&](TD_PosType pos, const TD_CardNum&uCardNum) {
		if (1 == uContinuityCount)
		{
			retCardPattern.push(tagCardPattern(eCardPattern, uCardNum, 1));
			bRet = true;
		}
		else
		{
			if (!arrCardExpect.get(pos + uContinuityCount - 1, [&](const TD_CardNum&uCardNum) {
				if (uCardNum - uCardNum + 1 == uContinuityCount)
				{
					retCardPattern.push(tagCardPattern(eCardPattern, uCardNum, 1));
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

bool enumCardPattern(ECardPattern eCardPattern, TD_CardNum uBaseCardNum = 0, UINT uContinuityCount=0)
{
	auto itr = g_mapCardPattern.find(eCardPattern);
	if (itr == g_mapCardPattern.end())
	{
		return false;
	}
	UINT uCardPatternFlag = itr->second;
	
	JSMap<TD_CardNum, UINT> mapCardSum;

	JSArray<tagCardPattern> retCardPattern;
	return enumCardPattern(eCardPattern, uCardPatternFlag, mapCardSum, uBaseCardNum, uContinuityCount, retCardPattern);
}

int main()
{
	JSMap<int, set<int>> jjhh;

	struct tag {};
	JSArray<tag> qqta{ {},{},{},{} };
	auto idof = qqta.indexOf({});
	qqta.sort();
	auto strsk = qqta.toString();
	
	JSArray<int> qq  ( {1,3,4,2 });
	qq= { 12,2,3 };
	auto aaaa = qq.indexOf({2});
	qq.sort();
	auto aaaaaaa = qq.toString();

	//qq = ll;
	auto idf = qq.indexOf(2);

	JSMap<int, int> masaa{ { 21,2 },{ 1,2 },{ 1,2 } };
	auto hbfdjv = masaa.toString();

	JSArray<int> arr(qq);
	arr=qq;
	arr.push({ 66,44,19,2,2,3,5,5,6,7,8 });
	qq.push( { 12,2,3 });
	JSArray<int> arr2;
	arr2 = arr.map([](float i) {return i * 2; });
	auto ff = arr.reduce(2.0f, [](const float& a1, const int& a2) {return a1+a2; });

	auto arr3 = arr2.sub(6, 3);

	map<int, int> tm;
	JSMap<int,int> mmm(tm);
	mmm = tm;
	mmm.keys();

	JSSet<int> set ;
	set.add({ 2,3,4 });
	set.add({ 1,2,3 });

	JSSet<int> set2 ( {1});
	set2 = arr2;
	set2.add({});

	JSMap<int, int> m{ { 21,2 },{ 1,2 },{ 1,2 } };
	auto as = m[1];
	m[1] *= 2;
	m.set(1, 1);

	enumCardPattern(CP_None);

	JSMap<int, int> ts1;
	ts1.assign( { { 1, 1 }, { 32,1 }, { 3,1 } });

	JSArray<int> ts2(7.9);
	ts2.sort();

	int uy = 1;
	PtrArray<int> PtrArray111(ts2);
	PtrArray111.push(uy);
	PtrArray111.push(&uy);
	PtrArray111.push({ &uy });
	PtrArray111.concat({ &uy });

	ConstPtrArray<int> ConstPtrArray111(uy);
	ConstPtrArray111.push(PtrArray111);
	ConstPtrArray111.concat(PtrArray111);

	ConstPtrArray<int> ConstPtrArray222(ConstPtrArray111);
	ConstPtrArray222.push(ConstPtrArray111);
	ConstPtrArray222.concat(ConstPtrArray111);

	return 0;
}
