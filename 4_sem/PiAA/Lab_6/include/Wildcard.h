#pragma once
#include <Bor.h>
#include <map>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::pair<std::wstring, int>> split(const std::wstring& str, wchar_t separator);

void addSplitParts(const std::wstring& str, wchar_t sep, Bor& mBor);

void searchPositionPattern(Bor& mBor, size_t patternSize, size_t textSize);

void printWildcard(Bor& mBor);