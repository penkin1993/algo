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
 */

class Graph {
public:
    explicit Graph(int n_vertices);

    ~Graph() = default;

    Graph(const Graph &) = default;

    Graph(Graph &&) = delete;

    Graph &operator=(const Graph &) = delete;

    Graph &operator=(Graph &&) = delete;

    void add(int vert_1, int vert_2);

    int count_path(int s_in, int s_out) const;

private:
    std::unordered_map<int, std::pair<std::vector<int>, int>>  bfs(int s) const;

    std::unordered_map<int, std::vector<int>> graph_structure; // вершина, ребра
};


Graph::Graph(int n_vertices) {
    for (int i = 0; i < n_vertices; i++) {
        graph_structure[i] = std::vector<int>(0);
    }
}

void Graph::add(int vert_1, int vert_2) {
    graph_structure[vert_1].push_back(vert_2);
    graph_structure[vert_2].push_back(vert_1);
}

std::unordered_map<int, std::pair<std::vector<int>, int>> Graph::bfs(int s) const { //
    std::queue<int> q; // буфер для вершин
    q.push(s);
    int v = 0;

    std::unordered_map<int, std::pair<std::vector<int>, int>> graph_path; // вершина, ребра, степень
    graph_path[s] = std::make_pair(std::vector<int>(), 0);

    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (const int adj_v : graph_structure.at(v)){
            if ( graph_path.find(adj_v) == graph_path.end() ) {
                // not found
                graph_path[adj_v] = std::make_pair(std::vector<int>{v}, graph_path[v].second + 1);
                // добавили вершину родителя и время
            } else if (graph_path.at(adj_v).second == graph_path[v].second + 1){
                // если уже есть вершина в словаре, но время прихода в данную вершину из корневой одинаковы
                graph_path[adj_v].first.push_back(v);
            }
        }
    }
    return graph_path;
}

int Graph::count_path(int s_in, int s_out) const {
    std::unordered_map<int, std::pair<std::vector<int>, int>> graph_path = bfs(s_in);

    int v, local_v = 0;
    std::vector<int> counter(graph_path[s_out].second + 1, 0);
    std::queue<int> q;
    q.push(s_out);
    while (!q.empty()) {
        v = q.front();
        q.pop();
        while (!graph_path[v].first.empty()){
            counter[graph_path[v].second]++;// ++ массив

            local_v = graph_path[v].first.front();
            graph_path[v].first.pop_back();
            q.push(local_v); // добавить элемент в очередь

        }
    }
    int multi = 1;
    for (const auto& e: counter)
        multi *= e;

    return multi;
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
    std::cout << graph.count_path(s_in, s_out);

    return 0;
}

/*
 Пример
Ввод	Вывод
////////////////////////////////////
4
5
0 1
0 2
1 2
1 3
2 3

0 3
2
////////////////////////////////////
3
3
0 1
0 2
1 2

0 1
1
////////////////////////////////////
5
7
0 1
0 2
1 2
1 3
2 3
3 4
2 4

0 4
1
//////////////////////////////////////
5
7
0 1
0 2
1 2
1 3
2 3
3 4
2 4

2 3
0
*/

