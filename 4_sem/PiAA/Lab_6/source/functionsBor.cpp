#include <Bor.h>


Node::Node(int _parent, wchar_t _symbol, size_t size) :
	parent(_parent), symb(_symbol), flag(false), suffixLink(-1)
{
	nextNode = std::vector<int>(size, -1);
	autoMove = std::vector<int>(size, -1);
};


Bor::Bor()
{ 
		// Расширение словаря (Поддрежка a-zA-Zа-яА-Я)
		uint16_t count = 0;
		for (wchar_t i = L'a'; i <= L'z'; i++)
		{
			alphabet[i] = count;
			count++;
		}
		for (wchar_t i = L'A'; i <= L'Z'; i++)
		{
			alphabet[i] = count;
			count++;
		}
		for (wchar_t i = L'а'; i <= L'я'; i++)
		{
			alphabet[i] = count;
			count++;
		}
		for (wchar_t i = L'А'; i <= L'Я'; i++)
		{
			alphabet[i] = count;
			count++;
		}
	nodesBor.push_back(createNode(0, '#'));
}

// Поиск подстроки
void Bor::searchPatterns(const std::wstring& text)
{
	int j = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		j = getAutoMove(j, parseAlphabet(text[i]));
		checkIndex(j, i + 1);
	}
}

// Добавление подстроки
void Bor::addPattern(const std::wstring& pattern)
{
	int j = 0;
	for (size_t i = 0; i <  pattern.size(); i++)
	{
		size_t letterIndex = parseAlphabet(pattern[i]);
		if (nodesBor[j].nextNode[letterIndex] == -1)
		{
			nodesBor.push_back(createNode(j, pattern[i]));
			nodesBor[j].nextNode[letterIndex] = nodesBor.size() - 1;
		}
		j = nodesBor[j].nextNode[letterIndex];
	}

	patterns.push_back(pattern);

	nodesBor[j].flag = true;
	nodesBor[j].indexPattern.push_back(patterns.size() - 1);
};

// Вывод результат
void Bor::printResult() const
{
	std::cout << "Result:\n";
	for (size_t i = 0; i < result.size(); i++)
		std::cout << result[i].first << " " <<
		        result[i].second << "\n";
}



Node Bor::createNode(int parent, wchar_t symbol)
{
	return { parent, symbol, alphabet.size() };
}

int Bor::getSuffixLink(int currentNode)
{
	if (nodesBor[currentNode].suffixLink == -1)
	{
		if (currentNode == 0 || nodesBor[currentNode].parent == 0)
			nodesBor[currentNode].suffixLink = 0;
		else
			nodesBor[currentNode].suffixLink = getAutoMove
			(getSuffixLink(nodesBor[currentNode].parent), parseAlphabet(nodesBor[currentNode].symb));
	}
	return nodesBor[currentNode].suffixLink;
}

int Bor::getAutoMove(int node, int character)
{
	if (nodesBor[node].autoMove[character] == -1)
	{
		if (nodesBor[node].nextNode[character] != -1)
			nodesBor[node].autoMove[character] = nodesBor[node].nextNode[character];
		else 
		{
			if (node == 0)
				nodesBor[node].autoMove[character] = 0;
			else
				nodesBor[node].autoMove[character] = getAutoMove(getSuffixLink(node), character);
		}
	}
	return nodesBor[node].autoMove[character];
}

void Bor::checkIndex(int startNode, int index)
{
	for (int currentNode = startNode; currentNode != 0; currentNode = getSuffixLink(currentNode))
	{
		if (nodesBor[currentNode].flag == true)
			for (const auto& num : nodesBor[currentNode].indexPattern)
				result.push_back(std::make_pair(index - patterns[num].size() + 1, num + 1));
	}
}

uint16_t Bor::parseAlphabet(wchar_t character)
{
	if (alphabet.count(character) != 0)
		return alphabet.at(character);
	else
		throw std::out_of_range("Not found character!");
}
