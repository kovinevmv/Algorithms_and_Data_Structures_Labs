#include "Bor.h"
#include "Wildcard.h"
#include <locale.h>
using namespace std;

int main() 
{
	::setlocale(LC_ALL, "ru_RU.UTF8");
	wcin.imbue(locale("ru_RU.UTF-8"));
    wcout.imbue(locale("ru_RU.UTF-8"));
	
	Bor mBor;
	wstring text, pattern;
	wchar_t wildcard;

	cout << "Enter text: ";
	wcin >> text;
	
	cout << "Enter pattern: ";
	wcin >> pattern;
	
	cout << "Enter wildcard: ";
	wcin >> wildcard;
	

	addSplitParts(pattern, wildcard, mBor);
	mBor.searchPatterns(text);

	searchPositionPattern(mBor, pattern.size(), text.size());
	printWildcard(mBor);
		
	return 0;
}