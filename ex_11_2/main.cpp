#include <iostream>
#include <unordered_map>
#include <deque>
#include <queue>

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

class Graph { // TODO: добавить топ 5
public:
    Graph(int n_vertices);

    void bfs(int s);

    void add(int &vert_1, int &vert_2);

    int call(int s_in, int s_out);

private:
    int counter_path = 0;

    void rev_bfs(int s_in, int s_out);

    std::unordered_map<int, std::pair<std::vector<int>, int>> graph_structure; // вершина, ребра, степень
};

Graph::Graph(int n_vertices) {
    for (int i = 0; i < n_vertices; i++) {
        graph_structure[i] = std::make_pair(std::vector<int>(0), -1);
    }
}

void Graph::add(int &vert_1, int &vert_2) {
    graph_structure[vert_1].first.push_back(vert_2);
    graph_structure[vert_2].first.push_back(vert_1);
    //std::cout << "add " << vert_1;
}

void Graph::bfs(int s) { // TODO: Освободить q
    std::queue<int> q; // буфер для вершин
    q.push(s);
    int counter = 1;
    int adj_v, v = 0;
    graph_structure[0].second = 0;
    while (!q.empty()) { // TODO: Более парвильный выход ????
        v = q.front();
        q.pop();
        for (int i = 0; i < graph_structure[v].first.size(); i++) {// итерируемся по всем смежным вершинам
            adj_v = graph_structure[v].first[i];
            if (graph_structure[adj_v].second == -1) { // получить все смежные вершины и записать их в стек
                graph_structure[adj_v].second = counter;
                //std::cout << adj_v << " " << counter << "\n";
                q.push(adj_v);
            }
        }
        counter++;
    }
}

void Graph::rev_bfs(int s_in, int s_out) {
    if (s_in == s_out) {
        //std::cout << "COUNTER \n";
        counter_path++;
    } else {
        int adj_v;
        for (int i = 0; i < graph_structure[s_out].first.size(); i++) {// итерируемся по всем смежным вершинам
            adj_v = graph_structure[s_out].first[i];
            if (graph_structure[adj_v].second == (graph_structure[s_out].second - 1)) {
                rev_bfs(s_in, adj_v);
            }
        }
        // найти смежные вершины с минимальным весом (на единицу меньше)
        // запустить рекрсивно данную функцию на них
    }
}


int Graph::call(int s_in, int s_out) {
    bfs(s_in);
    rev_bfs(s_in, s_out);
    return counter_path;
}

int main() {
    int n_vertices = 0;
    int n_edges = 0;

    std::cin >> n_vertices;
    std::cin >> n_edges;

    Graph graph = *new Graph(n_vertices);
    int vert_1;
    int vert_2;

    for (int i = 0; i < n_edges; i++) {
        std::cin >> vert_1 >> vert_2;
        graph.add(vert_1, vert_2);
    }

    int s_in, s_out;
    std::cin >> s_in >> s_out;
    std::cout << graph.call(s_in, s_out);

    return 0;
}