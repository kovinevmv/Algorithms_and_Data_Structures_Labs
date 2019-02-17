#include "functionsComparison.h"
using namespace std;

int main()
{
	srand(time(0));
	map<int, map<int, pair<int, int>>> result;

	int maxTextSize = 2097152;
	int maxPatternSize = 2048;
	double numberOfTests = 10.0;

	int allTestCount = log2(maxPatternSize) * ((log2(maxTextSize) - log2(maxPatternSize)) / 2 + 1);
	int currentTest = 0;

	cout << "Tests\nNaive search vs KMP algorithm\n";
	for (int i = maxPatternSize; i <= maxTextSize; i*=4)
	{
		for (int j = 2; j <= maxPatternSize; j*=2)
		{
			currentTest++;
			cout << "[" << currentTest << "/" << allTestCount << "] - ";
			cout << "Pattern size: " << j << "\n        - Text size: " << i << endl;
			int naive_count = 0;
			int kmp_count = 0;

			cout << "Processing\n";
			for (int k = 1; k <= numberOfTests; k++)
			{
				string pattern = generateString(j);
				string text = generateString(i);

				_naive(pattern, text, naive_count);
				_algoKnuthMorrisPratt(pattern, text, kmp_count);

				cout << ".";
			}

			naive_count /= numberOfTests;
			kmp_count /= numberOfTests;

			cout << "                OK" << endl;
			cout << "Comparisons (Naive/KMP):  " << naive_count << "/"
				<< kmp_count <<  "\n---------------------------------------\n";
			result[j][i] = make_pair(naive_count, kmp_count);
		}
	}

	cout << "\nResult Tables (See ./doc/results.md GitHub Markdown):\n\n";
	print1Table(result, cout);
	print2Table(result, cout);

	return 0;
}
