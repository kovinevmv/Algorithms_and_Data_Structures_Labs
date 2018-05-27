#include "Wildcard.h"
#include <gtest/gtest.h>
using namespace std;

struct AhoCorasickStruct
{
    vector<wstring> patterns;
    wstring text;
    vector<pair<int, int>> expectResult;
	
};

struct AhoCorasick_Test : testing::Test, testing::WithParamInterface<AhoCorasickStruct>
{    
    Bor mBor;
    vector<pair<int, int>> result;

    AhoCorasick_Test() {}

    void SetUp() override
	{
        for (const auto& pattern : GetParam().patterns)
            mBor.addPattern(pattern);

		mBor.searchPatterns(GetParam().text);
        result = mBor.result;
    }
};

TEST_P(AhoCorasick_Test, AhoCorasick_Tests){
    EXPECT_EQ(result, GetParam().expectResult);
}


INSTANTIATE_TEST_CASE_P(AhoCorasickTestInstantiate, AhoCorasick_Test,
testing::Values
(
    AhoCorasickStruct{ {L"NGT", L"GTA", L"AGT"}, L"NGTAGTGTA", {{1, 1}, {2, 2}, {4, 3}, {7, 2}} },
    AhoCorasickStruct{ {L"ACT", L"CTN", L"NCT"}, L"ACTNCTNCTACT", {{1, 1}, {2, 2}, {4, 3}, {5, 2}, {7, 3}, {10, 1}} },
    AhoCorasickStruct{ {L"NNNNNNN", L"AAAAAA", L"CCCC"}, L"AGTCATGCACACAC", {} },
	AhoCorasickStruct{ {}, L"", {} },   
    AhoCorasickStruct{ {L"C"}, L"", {} },
	AhoCorasickStruct{ {L""}, L"C", {} },
	AhoCorasickStruct{ {L"C"}, L"C", {{1,1}} },
	AhoCorasickStruct{ {L"C"}, L"CC", {{1,1}, {2,1}} },
	AhoCorasickStruct{ {L"C"}, L"CCC", {{1,1}, {2,1}, {3,1}} },
	AhoCorasickStruct{ {L"C", L"A"}, L"", {} },
	AhoCorasickStruct{ {L"C", L"A"}, L"C", {{1,1}} },
	AhoCorasickStruct{ {L"C", L"A"}, L"A", {{1,2}} },
	AhoCorasickStruct{ {L"C", L"A"}, L"AC", {{1,2},{2,1}} },

	//EXTRA
	AhoCorasickStruct{ {L"й"}, L"tьй", {{3,1}} }	
));

// ----------------------------------------------------------------------------------------


struct WildCardStruct
{
	wstring pattern;
    wstring text;
	char joker;
    vector<int> expectResult;
	
};

struct WildCardStruct_Test : testing::Test, testing::WithParamInterface<WildCardStruct>
{    
    Bor mBor;
    vector<int> result;

    WildCardStruct_Test() {}

    void SetUp() override
	{
		addSplitParts(GetParam().pattern, GetParam().joker, mBor);
	    mBor.searchPatterns(GetParam().text);
	    searchPositionPattern(mBor, GetParam().pattern.size(), GetParam().text.size());
		result = mBor.wildcardResult;
    }
};

TEST_P(WildCardStruct_Test, WildCardStruct_Tests){
    EXPECT_EQ(result, GetParam().expectResult);
}


INSTANTIATE_TEST_CASE_P(AhoCorasickTestInstantiate, WildCardStruct_Test,
testing::Values
(
    WildCardStruct{ L"CC", L"C", L'#', {} },   
    WildCardStruct{ L"C", L"AAAA", L'C', {1,2,3,4} }, 
    WildCardStruct{ L"CC", L"TTTT", L'C', {1,2,3,4} }, 	
    WildCardStruct{ L"CC", L"CCCA", L'!', {1,2} },   
    WildCardStruct{ L"A.T", L"ACTATTATT", L'.', {1,4,7} },   
    WildCardStruct{ L",,A", L"AAAAA", L',', {1,2,3} }, 
    WildCardStruct{ L".", L"ACTGAGTAC", L'.', {1,2,3,4,5,6,7,8,9} },
	WildCardStruct{ L"", L"", L'!', {} },   
    WildCardStruct{ L"A", L"", L'4', {} },   
    WildCardStruct{ L"T", L"T", L'3', {1} }
));

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
