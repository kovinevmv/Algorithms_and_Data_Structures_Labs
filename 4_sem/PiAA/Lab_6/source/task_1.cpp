#include "Bor.h"
using namespace std;

int main() 
{
	::setlocale(LC_ALL, "ru_RU.UTF8");
	wcin.imbue(locale("ru_RU.UTF-8"));
    wcout.imbue(locale("ru_RU.UTF-8"));
	wstring text, patterns;
	size_t size;

	Bor mBor;
	
	cout << "Enter text: ";
	wcin >> text;
	
	cout << "Enter number of patterns: ";
	wcin >> size;
	

	
	for (size_t i = 0; i < size; i++)
	{
		cout << "Enter " << i+1 << " pattern: ";
		wcin >> patterns;
		mBor.addPattern(patterns);
	}

	mBor.searchPatterns(text);
	mBor.printResult();
	
	return 0;
}
