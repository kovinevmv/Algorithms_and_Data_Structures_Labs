#include "functionsString.h"

int main()
{
	string text, pattern;
	cout << "Knuth Morris Pratt Algorithm\n\n";
	cout << "Enter pattern (|P| < 15000): ";
	cin >> pattern;

	cout << "Enter text (|T| < 5000000): ";
	cin >> text;

	vector<int> result = algoKnuthMorrisPratt(pattern, text);

	cout << "Result:\nFound pattern indexes: ";
	for (int i = 0; i < result.size() - 1; i++)
		cout << result[i] << ",";

	cout << result[result.size() - 1] << endl;
    	return 0;
}
