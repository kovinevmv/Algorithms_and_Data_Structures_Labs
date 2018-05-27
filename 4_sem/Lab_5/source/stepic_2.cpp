#include "functionsString.h"


int main()
{
	string text, pattern;
	cout << "Search shift\n\n";
	cout << "Enter first string:  ";
	cin >> pattern;

	cout << "Enter second string: ";
	cin >> text;

	cout << "Found index: " << searchShift(pattern, text) << endl;

	return 0;
}
