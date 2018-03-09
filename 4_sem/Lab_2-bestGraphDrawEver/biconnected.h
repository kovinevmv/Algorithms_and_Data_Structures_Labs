#include<iostream>
#include <list>
#define NIL -1
using namespace std;

class Graph
{
    int V;
    list<int> *adj;
    bool isBCUtil(int v, bool visited[], int disc[], int low[],
                 int parent[]);
public:
    Graph(int V);
    void addEdge(int v, int w);
    bool isBC();
};
