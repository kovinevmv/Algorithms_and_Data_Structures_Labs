#include <iomanip>
#include <random>
#include <iostream>
#include <vector>
#include <string>
#include <ctime> 
#include <map>
using namespace std;

vector<int> _prefixFunction(const string& text, int& count);

vector<int> _algoKnuthMorrisPratt(const string& pattern, const string& text, int& count);

vector<size_t> _naive(const string& pattern, const string& text, int& count);

string generateString(int size);

void print1Table(map<int, map<int, pair<int, int>>>& result, ostream& out);

void print2Table(map<int, map<int, pair<int, int>>>& result, ostream& out);