#include "biconnected.h"
#define NIL -1
using namespace std;

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

bool Graph::isBCUtil(int u, bool visited[], int disc[],int low[],int parent[])
{
    static int time = 0;
    int children = 0;

    visited[u] = true;
    disc[u] = low[u] = ++time;

    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;
        if (!visited[v])
        {
            children++;
            parent[v] = u;


            if (isBCUtil(v, visited, disc, low, parent))
               return true;

            low[u]  = min(low[u], low[v]);

            if (parent[u] == NIL && children > 1)
               return true;
            if (parent[u] != NIL && low[v] >= disc[u])
               return true;
        }
        else
            if (v != parent[u])
                low[u]  = min(low[u], disc[v]);
    }
    return false;
}

bool Graph::isBC()
{

    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];

    for (int i = 0; i < V; i++)
    {
        parent[i] = NIL;
        visited[i] = false;
    }


    if (isBCUtil(0, visited, disc, low, parent) == true)
        return false;

    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            return false;

    return true;
}
