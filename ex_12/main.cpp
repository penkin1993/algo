#include <iostream>
#include <unordered_map>
#include <deque>
#include <queue>

/*
Мосты.
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.

Ввод: Первая строка входного файла содержит два целых числа
n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000).
Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается двумя натуральными числами bi,
ei — номерами концов ребра (1 ≤ bi, ei ≤ n).
Вывод: Первая строка выходного файла должна содержать одно натуральное число b — количество мостов в заданном графе.
На следующей строке выведите b целых чисел — номера ребер, которые являются мостами, в возрастающем порядке.
Ребра нумеруются с единицы в том порядке, в котором они заданы во входном файле.
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

void Graph::bfs(int s) { //
    std::queue<int> q; // буфер для вершин
    q.push(s);
    int adj_v, v = 0;
    graph_structure[s].second = 0;
    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (int i = 0; i < graph_structure[v].first.size(); i++) {// итерируемся по всем смежным вершинам
            adj_v = graph_structure[v].first[i];
            if (graph_structure[adj_v].second == -1) { // получить все смежные вершины и записать их в стек
                graph_structure[adj_v].second = graph_structure[v].second + 1;
                //std::cout << adj_v << " " << graph_structure[adj_v].second << "\n";
                q.push(adj_v);
            }
        }
    }
}


void Graph::rev_bfs(int s_in, int s_out) {
    if(s_in == s_out){
        counter_path++;
        return;
    }
    std::queue<int> q; // буфер для вершин
    int adj_v, v;
    q.push(s_out);

    while(!q.empty()) {
        v = q.front();
        q.pop();

        for (int i = 0; i < graph_structure[v].first.size(); i++) {// итерируемся по всем смежным вершинам
            adj_v = graph_structure[v].first[i];
            //std::cout << "Compare " << adj_v << " " <<  graph_structure[adj_v].second << "; " << v << " " << graph_structure[v].second << "\n";
            if (graph_structure[adj_v].second == (graph_structure[v].second - 1)) {
                if (adj_v == s_in){
                    counter_path++;
                } else{
                    q.push(adj_v);
                }
            }
        }
    }
}


int Graph::call(int s_in, int s_out) {
    bfs(s_in);
    rev_bfs(s_in, s_out);
    return counter_path;
}

int main() { // TODO: Кратные ребра !!!
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


