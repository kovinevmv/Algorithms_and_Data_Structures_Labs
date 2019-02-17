#pragma once
#include <map>
#include <iostream>
#include <vector>
#include <string>

using alphabetMap = std::map<wchar_t, uint16_t>;

class Node
{
public:
	// Конструктор класса Node
	Node(int _parent, wchar_t _symbol, size_t size);

	int  parent;           // Родитель (вершина)
	wchar_t symb;             // Символ на ребре от parent к этой вершине
	bool flag;             // Проверка вершины на строку
	int  suffixLink;       // Суффиксная ссылка

	std::vector<int> nextNode;          // Массив номеров вершин переходов по символу алфавита
	std::vector<int> indexPattern;      // Номера подстрок в данной вершине
	std::vector<int> autoMove;	        // Запоминание перехода автомата
};

class Bor
{
public:
	// Конструктор класса
	Bor();

	// Поиск подстроки
	void searchPatterns(const std::wstring& text);

	// Добавление подстроки
	void addPattern(const std::wstring& pattern);
	
	// Вывод резлуьтат
	void printResult() const;
	
	std::vector<std::pair<int, int>> result;  // Индексы вхождений найденных подстрок в тексте

	std::vector<Node>   nodesBor;            // Вершины бора      
	std::vector<std::wstring> patterns;       // Массив введенных подстрок
	
	// Массив начaльных позиции подстрок в шаблоне с wildcards
	std::vector<int> positionsPatternWildcard;
	std::vector<int> vectorWildcards;     // Массив для поиска строки с wildcards
	
	std::vector<int> wildcardResult;     // Результат поиска

	alphabetMap alphabet;

private:

	Node createNode(int parent, wchar_t symbol);

	int getSuffixLink(int currentNode);

	int getAutoMove(int node, int character);

	void checkIndex(int startNode, int index);
	
	uint16_t parseAlphabet(wchar_t character);
};
