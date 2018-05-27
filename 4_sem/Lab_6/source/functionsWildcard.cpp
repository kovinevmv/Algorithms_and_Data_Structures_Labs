#include "Wildcard.h"

std::vector<std::pair<std::wstring, int>> split(const std::wstring& str, wchar_t separator)
{
	std::wstring temp;
	std::vector<std::pair<std::wstring, int>> splitStrings;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] != separator)
		{
			temp.push_back(str[i]);
			if (str[i + 1] == separator || str[i + 1] == '\0')
			{
				splitStrings.push_back(make_pair(temp, i - temp.length() + 2));
				temp.clear();
			}
		}
	}

	return splitStrings;
}

void addSplitParts(const std::wstring& str, wchar_t sep, Bor& mBor)
{
	std::vector<std::pair<std::wstring, int>> splitStrings = split(str, sep);
	for (size_t i = 0; i < splitStrings.size(); i++)
	{
		mBor.addPattern(splitStrings[i].first);
		mBor.positionsPatternWildcard.push_back(splitStrings[i].second);
	}
}

void searchPositionPattern(Bor& mBor, size_t patternSize, size_t textSize)
{
	mBor.vectorWildcards.resize(textSize);
	for (size_t i = 0; i < mBor.result.size(); ++i)
	{
		int textPosition = mBor.result[i].first - 1;
		int partPosition = mBor.positionsPatternWildcard[mBor.result[i].second - 1];
		int index = textPosition - partPosition + 1;
		
		if (index + patternSize > textSize)
			continue;

		if (textPosition > partPosition - 2)
			mBor.vectorWildcards[index]++;
	}
	for (size_t i = 0; i < mBor.vectorWildcards.size(); ++i)
		if (mBor.vectorWildcards[i] == static_cast<int>(mBor.patterns.size()))
				mBor.wildcardResult.push_back(i+1);
}

void printWildcard(Bor& mBor) 
{
	std::cout << "Founded indexes: ";
	for (size_t i = 0; i < mBor.wildcardResult.size(); i++)
		std::cout << mBor.wildcardResult[i] << " ";
	std::cout << "\n";
}

