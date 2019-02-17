#include "functionsString.h"
#include <gtest/gtest.h>


TEST(Prefix_Test, all_different_symbols)
{
	auto result = prefixFunction("abcdefghijklmnopqrstuvwxyz");
	vector<int> correct = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	EXPECT_EQ(result, correct);
}
TEST(Prefix_Test, string_with_one_main_symbol)
{
	auto result = prefixFunction("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
	vector<int> correct = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
							26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};
	EXPECT_EQ(result, correct);
}


TEST(KMP_Test, similiar)
{
	auto result = algoKnuthMorrisPratt("12345", "12345");
	vector<int> correct = {0};
	EXPECT_EQ(result, correct);
}
TEST(KMP_Test, all_index)
{
	auto result = algoKnuthMorrisPratt("1", "1111111111111111111111111111");
	vector<int> correct = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
	EXPECT_EQ(result, correct);
}
TEST(KMP_Test, real_text)
{
	auto result = algoKnuthMorrisPratt("cmake", 
		"CMake is a cross-platform free and open-source software for managing the build process of software using a compiler-independent "
		"method. It supports directory hierarchies and applications that depend on multiple libraries. It is used in conjunction with    "
		"native build environments such as make, Apple's Xcode, and Microsoft Visual Studio. It has minimal dependencies, requiring only "
		"a C++ compiler on its own build system.\nFeatures\nCMake can handle in-place and out-of-place builds, enabling several builds   "
		"from the same source tree, and cross-compilation. The ability to build a directory tree outside the source tree is a key        "
		"feature, ensuring that if a build directory is removed, the source files remain unaffected.\nCMake can locate executables,      "
		"files, and libraries. These locations are stored in a cache, which can then be tailored before generating the target build      "
		"files. The cache can be edited with a graphical editor which is included in the project.\nComplicated directory hierarchies and "
		"applications that rely on several libraries are well supported by CMake. For instance, CMake is able to accommodate a project   "
		"that has multiple toolkits, or libraries that each have multiple directories. In addition, CMake can work with projects that    "
		"require executables to be created before generating code to be compiled for the final application. Its open-source, extensible  "
		"design allows CMake to be adapted as necessary for specific projects.[4]\nCMake can generate project files for several prominent"
		"IDEs, such as Microsoft Visual Studio, Xcode, and Eclipse CDT. It can also produce build scripts for MSBuild or NMake on        "
		"Windows; Unix Make on Unix-like platforms such as Linux, macOS, and Cygwin; and Ninja on both Windows and Unix-like             "
		"platforms.\nBuild process\nThe build process with CMake takes place in two stages. First, standard build files are created from "
		"configuration files. Then the platform's native build tools are used for the actual building.[4]\nEach build project contains a "
		"CMakeLists.txt file in every directory that controls the build process. The CMakeLists.txt file has one or more commands in the "
		"form COMMAND (args...), with COMMAND representing the name of each command and args the list of arguments, each separated by    "
		"white space. While there are many built-in rules for compiling the software libraries (static and dynamic) and executables,     "
		"there are also provisions for custom build rules. Some build dependencies can be determined automatically. Advanced users can   "
		"also create and incorporate additional makefile generators to support their specific compiler and OS needs.\nHistory\nCMake     "
		"development began in 1999 in response to the need for a cross-platform build environment for the Insight Segmentation and       "
		"Registration Toolkit (ITK).[5] The project is funded by the United States National Library of Medicine as part of the Visible   "
		"Human Project. It was partially inspired by pcmaker, which was made by Ken Martin and other developers to support the           "
		"Visualization Toolkit (VTK). At Kitware, Bill Hoffman blended components of pcmaker with his own ideas, striving to mimic the   "
		"functionality of Unix configure scripts. CMake was first implemented in 2000 and further developed in 2001. Continued           "
		"development and improvements were fueled by the incorporation of CMake into developers’ own systems, including the VXL          "
		"Project[clarification needed], The CABLE[6] features added by Brad King,[clarification needed] and GE Corporate R&D for support "
		"of DART.[clarification needed]\nAdditional features were created when VTK transitioned to CMake for its build environment and   "
		"for supporting ParaView.\nCPack\nCPack is a packaging system for software distributions which is tightly integrated with CMake, "
		"but can function without it.\nIt can be used to generate:\nLinux RPM, deb and gzip distributions of both binaries and source    "
		"code.\nNSIS files (for Microsoft Windows)\nMac OS X packages                                                                    ");
	vector<int> correct = {2979,3139};
	EXPECT_EQ(result, correct);
}
TEST(KMP_Test, overlay)
{
	auto result = algoKnuthMorrisPratt("wowowowo", "wowowowowowowowo");
	vector<int> correct = {0,2,4,6,8};
	EXPECT_EQ(result, correct);
}


TEST(Shift_Test, similiar)
{
	std::string first = "aaaaaa";
	std::string second = "aaaaaa";
	auto result = searchShift(first, second);
	EXPECT_EQ(result, 0);
}
TEST(Shift_Test, long_string)
{
	std::string first = 
		"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"
		"ycy1aPG90VlAvkiJ2QXX";
	std::string second = 
		"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"
		"bwFd49hpYarUgHfCdhAn";
		
	auto result = searchShift(first, second);
	EXPECT_EQ(result, 892);
}


struct allExpectedData
{
	string pattern;
	string text;

 	
	vector<int> expectKMP;
	vector<int> expectPrefix;
	int expectShift;

};

struct allResultData : testing::Test, testing::WithParamInterface<allExpectedData>
{
	vector<int> resultKMP;
	vector<int> resultPrefix;
	int resultShift; 

	void SetUp() override
	{
		auto pattern = GetParam().pattern;
		auto text = GetParam().text;
		
		resultKMP = algoKnuthMorrisPratt(pattern, text);
		resultPrefix = prefixFunction(pattern);	
		resultShift = searchShift(text, pattern);
	}
};

TEST_P(allResultData, KMPTest)
{
	EXPECT_EQ(resultPrefix, GetParam().expectPrefix);
	EXPECT_EQ(resultKMP,    GetParam().expectKMP);
	EXPECT_EQ(resultShift,  GetParam().expectShift);
}

INSTANTIATE_TEST_CASE_P(KMPTests, allResultData,
testing::Values
(
	allExpectedData{"z", "", {-1}, {0}, -1},
	allExpectedData{"", "z", {-1}, {}, -1},
	allExpectedData{"zz", "", {-1}, {0, 1}, -1},
	allExpectedData{"", "zz", {-1}, {}, -1},
	allExpectedData{"z", "azazazazaz", {1,3,5,7,9}, {0}, -1},
	allExpectedData{"abcd", "abcdabcdabcdabcd", {0,4,8, 12}, {0, 0, 0, 0}, -1},
	allExpectedData{"defabc", "abcdef", {-1}, {0, 0, 0, 0, 0, 0}, 3}
	
));
int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
