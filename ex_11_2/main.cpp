#include <iostream>
#include <unordered_map>
#include <deque>

/*
11_2. Количество различных путей
Ограничение времени	0.2 секунды
Ограничение памяти	10Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
 Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).

Формат ввода
v: кол-во вершин (макс. 50000),
n: кол-во ребер(макс. 200000),
n пар реберных вершин,
пара вершин (u, w) для запроса.

Формат вывода
Количество кратчайших путей от v к w.

Пример
Ввод	Вывод
4
5
0 1
0 2
1 2
1 3
2 3

0 3
2

 */

class Graph
{
public:
    Graph(int n_vertices);
    void add(int & vert_1, int & vert_2);
private:
    std::unordered_map<int, std::pair<std::deque<int>, int>> graph_structure; // вершина, ребра, степень
};

Graph::Graph(int n_vertices) {
    for (int i = 0; i < n_vertices; i++){
        graph_structure[i] = std::make_pair(std::deque<int>(0), 0);
    }
}

void Graph::add(int & vert_1, int & vert_2) {
    graph_structure[vert_1].first.push_front(vert_2);
}

// TODO: void bfs()

int main() {
    int n_vertices = 0;
    int n_edges = 0;

    std::cin >> n_vertices;
    std::cin >> n_edges;

    Graph graph = * new Graph(n_vertices);
    int vert_1;
    int vert_2;

    for(int i; i < n_edges; i++){
        std::cin >> vert_1 >> vert_2;
        graph.add(vert_1, vert_2);

    }

    return 0;
}