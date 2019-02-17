#include "functionsComparison.h"
using namespace std;

vector<int> _prefixFunction(const string& text, int& count)
{
	if (text.empty())
		return {};

	vector<int> prefixVector(text.size(), 0);

	for (int i = 1; i < text.size(); i++)
	{
		int k = prefixVector[i - 1];

		while (k > 0 and text[i] != text[k])
		{
			count++;
			k = prefixVector[k - 1];
		}
			
		count++;
		if (text[i] == text[k])
			k++;

		prefixVector[i] = k;
	}

	return prefixVector;
}

vector<int> _algoKnuthMorrisPratt(const string& pattern, const string& text, int& count)
{
	vector<int> resultVector;
	vector<int> prefixVector = _prefixFunction(pattern, count);

	if (pattern.empty() || text.empty())
		return { -1 };

	int pos = 0;

	for (int i = 0; i < text.size(); i++) 
	{
		while (pos > 0 and (pos >= pattern.size() || pattern[pos] != text[i]))
		{
			count++;
			pos = prefixVector[pos - 1];
		}
			
		count++;
		if (pattern[pos] == text[i])
			pos++;

		if (pos == pattern.size())
			resultVector.push_back(i - pos + 1);
	}

	if (resultVector.empty())
		resultVector.resize(1, -1);

	return resultVector;
}

vector<size_t> _naive(const string& pattern, const string& text, int& count)
{
	if (!pattern.size() || !text.size()) 
		return {};

	vector<size_t> resultVector;
	
	for (size_t i = 0; i < text.size() - pattern.size() + 1; i++) 
	{
		for (size_t j = 0; j < pattern.size(); j++) 
		{
			count++;
			if (pattern[j] == text[j + i])
			{
				if (j == pattern.size() - 1)
					resultVector.push_back(i);
			}
			else
				break;

		}
	}
	return resultVector;
}

string generateString(int size)
{
	string result;
	for (int i = 0; i < size; i++)
	{

		result += rand() % 26 + 'A';
	}

	return result;
}

void print1Table(map<int, map<int, pair<int, int>>>& result, ostream& out)
{
	out << "|  P\\T |";
	for (auto& x : result[2])
		out << setw(18) << setfill(' ') << x.first << "  |";
	out << "\n|-----:|";
	for (int i = 0; i < result[2].size(); i++)
		out << "--------------------|";
	out << endl;


	for (auto& t : result)
	{
		out << "| " << right << setw(4) << setfill(' ') << t.first << " |";
		for (auto& x : t.second)
		{
			out << setw(8) << setfill(' ') << x.second.first;
			out << ";";
			out << setw(9) << setfill(' ') << x.second.second << "  |";
		}
		out << endl;
	}
	out << endl;
}

void print2Table(map<int, map<int, pair<int, int>>>& result, ostream& out)
{
	out << "|  P\\T |";
	for (auto& x : result[2])
		out << setw(18) << setfill(' ') << x.first << "  |";
	out << "\n|-----:|";
	for (int i = 0; i < result[2].size(); i++)
		out << "--------------------|";
	out << endl;

	for (auto& t : result)
	{
		out << "| " << right << setw(4) << setfill(' ') << t.first << " | ";
		for (auto& x : t.second)
		{
			if (x.second.first < x.second.second)
			{
				out << right << setw(8) << setfill(' ') << "Naive - ";
				out << right << setw(9) << abs(x.second.first - x.second.second) << "  | ";
			}
			else
			{
				out << right << setw(8) << setfill(' ') << "KMP  - ";
				out << right << setw(9) << abs(x.second.first - x.second.second) << "  | ";
			}
		}
		out << endl;
	}
}