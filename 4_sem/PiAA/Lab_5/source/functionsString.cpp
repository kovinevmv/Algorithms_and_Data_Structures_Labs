#include "functionsString.h"

vector<int> prefixFunction(const string& text)
{
	if (text.empty())
		return {};

	vector<int> prefixVector(text.size(), 0);

	for (int i = 1; i < text.size(); i++)
	{
		int k = prefixVector[i - 1];

		while (k > 0 and text[i] != text[k])
			k = prefixVector[k - 1];
			
		if (text[i] == text[k])
			k++;
			
		prefixVector[i] = k;
	}

	return prefixVector;
}

vector<int> algoKnuthMorrisPratt(const string& pattern, const string& text)
{
	vector<int> resultVector;
	vector<int> prefixVector = prefixFunction(pattern);

	if (pattern.empty() || text.empty())
		return { -1 };

	int pos = 0;

	for (int i = 0; i < text.size(); i++) 
	{
		while (pos > 0 and (pos >= pattern.size() || pattern[pos] != text[i]))
		{
			pos = prefixVector[pos - 1];
		}
			
		if (pattern[pos] == text[i])
			pos++;

		if (pos == pattern.size())
			resultVector.push_back(i - pos + 1);
	}

	if (resultVector.empty())
		resultVector.resize(1, -1);

	return resultVector;
}


int searchShift(const string& text, const string& pattern)
{
	int result = -1;

	
	if (text.size() != pattern.size())
		return result;

	vector<int> prefix  = algoKnuthMorrisPratt(pattern, text+text);
	
	if (prefix.empty())
		return { -1 };
	else
		return prefix[0];
}
