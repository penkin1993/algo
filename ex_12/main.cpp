#include <iostream>
#include <unordered_map>
#include <deque>
#include <queue>
#include <fstream>

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
    Graph(int n_vertices, std::priority_queue<int, std::vector<int>, std::greater<>> &bridges_) : bridges(bridges_) {
        for (int i = 1; i < n_vertices + 1; i++) {
            graph_structure[i] = std::vector<int>(0);
            //child[i] = std::vector<int>(0);
            //parent[i] = -1;
            time[i] = -1;
            //in_queue[i] = false;
        }
    }

    void add(int &vert_1, int &vert_2, int &ord);

    std::priority_queue<int, std::vector<int>, std::greater<>> call();

private:
    std::priority_queue<int, std::vector<int>, std::greater<>> &bridges;
    std::unordered_map<int, std::vector<int>> graph_structure; // вершина, ребра
    std::unordered_map<int, std::vector<int>> order;  // вершина порядок !!!
    std::unordered_map<int, int> time; // если время отрицательно, то цвет белый
    //std::unordered_map<int, std::vector<int>> child;  // цвет вершины. false - серый. true - черный
    std::unordered_map<int, int> parent;
    //std::unordered_map<int, bool> in_queue;

    void dfs(int s);
};

void Graph::add(int &vert_1, int &vert_2, int &ord) {
    graph_structure[vert_1].push_back(vert_2);
    graph_structure[vert_2].push_back(vert_1);
    order[vert_1].push_back(ord + 1);
    order[vert_2].push_back(ord + 1);
}


void Graph::dfs(int s) {
    int abs_time = 0; // время
    //int parent = -1; // TODO: Кто чей потомок !!!! ????

    std::stack<int> q_white; // буфер для белых вершин
    q_white.push(s);

    std::stack<int> q_grey; // буфер для серых вершин
    q_grey.push(s);


    //in_queue[s] = true; // добавлена в очередь как белая вершина
    int adj_v, v = 0;
    //graph_structure[s].second = 0;
    while (!q_white.empty()) {
        v = q_white.top();
        //std::cout << "\n";
        //std::cout << "v " << v << "\n";
        q_white.pop();

       // for (std::pair<int, int> element : time) {
       //     std::cout << "time : " << element.first << " " << element.second << "\n";
       // }
        //std::cout << "\n";

        if (time[v] == -1) { // Если врешина белая,
            time[v] = abs_time; // красим в серый
            q_grey.push(v); // добавляем в стек.
            abs_time++; // Увеличиваем время

            for (int i = 0; i < graph_structure[v].size(); i++) {
                adj_v = graph_structure[v][i];
                if (time[adj_v] == -1){
                    parent[adj_v] = v; // TODO: Неправильно !!!
                    std::cout << " adj_v " << adj_v << " v " << parent[v] << "\n";

                    q_white.push(adj_v); // добавлям в стек

                } else if ((time[adj_v] != -1) && (parent[v] != adj_v))  {
                    if (time[adj_v] < time[v]) {
                        time[v] = time[adj_v]; // то меняем время на меньшее
                    }
                }
            }
        }
    }

    while (!q_grey.empty()) {
        v = q_grey.top();
        q_grey.pop();
        //std::cout << "v " << v << "\n";

        for (int i = 0; i < graph_structure[v].size(); i++) {
            adj_v = graph_structure[v][i];
            if ((parent[v] != adj_v) && (time[adj_v] < time[v])){
                time[v] = time[adj_v]; // то меняем время на меньшее
            }
        }
        if (time[parent[v]] < time[v]){
            bridges.push(order[v][parent[v]]);
        }
    }
}






std::priority_queue<int, std::vector<int>, std::greater<>> Graph::call() {
    for (std::pair<int, std::vector<int>> element : graph_structure) {
        if (time[element.first] == -1) {
            dfs(element.first);
        }
    }
    return bridges;
}


int main() {
    std::ifstream input_file("bridges.in");
    std::ofstream output_file("bridges.out");
    std::priority_queue<int, std::vector<int>, std::greater<>> bridges;
    int n_vertices;
    int n_edges;

    input_file >> n_vertices >> n_edges;
    Graph graph = *new Graph(n_vertices, bridges);

    std::vector<int64_t> arr;
    std::string line;

    int v1, v2;
    for (int i = 0; i < n_edges; i++) {
        input_file >> v1 >> v2;
        graph.add(v1, v2, i);
    }
    input_file.close();

    graph.call();

    int size = bridges.size();
    output_file << size << "\n";
    for (int i = 0; i < size; i++) {
        output_file << bridges.top() << "\n";
        bridges.pop();
    }
    output_file.close();

    return 0;
}



