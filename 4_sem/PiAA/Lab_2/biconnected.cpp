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


// Рекурсивная функция, возвращающая true, если есть точка
// сочлинения в заданном графе, иначе возвращает false.

// u -> Вершина, которую нужно посетить следующей
// visited[] -> сохраняет трассировку посещенных вершин
// disc[] -> Сохраняет время обнаружения посещенных вершин
// parent [] -> Сохраняет родительские вершины в дереве DFS
bool Graph::isBCUtil(int u, bool visited[], int disc[],int low[],int parent[])
{
    static int time = 0;

    // Количество детей в дереве DFS
    int children = 0;

    // Отметить текущий узел как посещенный
    visited[u] = true;

    // Инициализировать время обнаружения и низкое значение
    disc[u] = low[u] = ++time;

    // Пройти через все вершины, смежные с этим
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i;

        // Если v еще не посещен, сделать его дочерним элементом u
        // в дереве DFS и повторить
        if (!visited[v])
        {
            children++;
            parent[v] = u;

            // проверяем, имеет ли подграф, связанный с v, точку сочленения
            if (isBCUtil(v, visited, disc, low, parent))
               return true;

            // Проверяем, имеет ли поддерево, связанное с v, соединение с
            // один из предков u
            low[u]  = min(low[u], low[v]);

            if (parent[u] == NIL && children > 1)
               return true;
            if (parent[u] != NIL && low[v] >= disc[u])
               return true;
        }
        else

            // Обновляем низкое значение u для вызовов родительских функций.
            if (v != parent[u])
                low[u]  = min(low[u], disc[v]);
    }
    return false;
}


// Основная функция, возвращающая true, если граф двусвязен,
// иначе false. Он использует рекурсивную функцию isBCUtil()
bool Graph::isBC()
{

    // Отметим все вершины как не посещенные
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];


    // Инициализация parent и visited
    for (int i = 0; i < V; i++)
    {
        parent[i] = NIL;
        visited[i] = false;
    }


    // Вызов рекурсивной вспомогательной функции, чтобы найти, если есть точка
    // сочлинения в заданном графе. Мы обходим DFS от вершины 0
    if (isBCUtil(0, visited, disc, low, parent) == true)
        return false;


    // Теперь проверим, связный ли данный граф или нет. Неориентированный
    // граф связан, если все вершины достижимы с любой стартовой
    // вершины (0 в качестве отправной точки в данном случае)
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            return false;

    return true;
}
