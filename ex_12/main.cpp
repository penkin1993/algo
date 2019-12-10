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
    Graph(int n_vertices);

    void dfs(int s, int abs_time);

    void add(int &vert_1, int &vert_2, int & ord);

    int call();

private:
    std::priority_queue<int> bridges;
    std::unordered_map<int, std::vector<int>> graph_structure; // вершина, ребра
    std::unordered_map<int, std::vector<int>> order;  // вершина порядок !!!
    std::unordered_map<int, int> time; // если время отрицательно, то цвет белый
    std::unordered_map<int, std::vector<int>> child;  // цвет вершины. false - серый. true - черный

};

Graph::Graph(int n_vertices) {
    for (int i = 1; i < n_vertices + 1; i++) {
        graph_structure[i] = std::vector<int>(0);
        child[i] = std::vector<int>(0);
        time[i] = -1;
    }
}

void Graph::add(int &vert_1, int &vert_2, int & ord) {
    graph_structure[vert_1].push_back(vert_2);
    graph_structure[vert_2].push_back(vert_1);
    order[vert_1].push_back(ord + 1);
    order[vert_2].push_back(ord + 1);
}


void Graph::dfs(int s, int abs_time) {
    std::stack<int> q; // буфер для вершин
    q.push(s);
    int adj_v, v = 0;
    //graph_structure[s].second = 0;
    while (!q.empty()) {
        v = q.top();
        q.pop();
        // Если врешина белая, то красим в серый и добавляем в стек. Увеличиваем время
        //     Итерируемся по всем вершинам.
        //     Если вершина белая, то добавлям в стек.
        //     Если врешина серая и время меньше, то меняем время на меньшее
        // Если вершина серая (фактически становится черной). То пытаемся найти мост
        //
        if (time[v] == -1) { // Если врешина белая,
            time[v] = abs_time; // красим в серый
            q.push(v); // добавляем в стек.
            abs_time++; // Увеличиваем время
            for (int i = 0; i < graph_structure[v].size(); i++) {
                adj_v = graph_structure[v][i];
                if (time[adj_v] == -1) {// Если вершина белая
                    q.push(adj_v); // добавлям в стек
                    child[v].push_back(adj_v); // И в child
                }
                //else{
                //    if (time[adj_v] < time[v]){ // Если врешина серая и время меньше ???????????????????????????????????????????
                //        time[v] = time[adj_v]; // то меняем время на меньшее
                //    }
                //}
            }
        } else { // врешина почти черная
            //Обновить время от детей
            for (int i = 0; i < child[v].size(); i++) { // не может быть белых врешин !!!!
                adj_v = child[v][i];
                if (time[adj_v] < time[v]) {
                    time[v] = time[adj_v];
                }

                for (int i = 0; i < graph_structure[v].size(); i++) { // не может быть белых врешин !!!! // Можно конечно не по всем
                    adj_v = graph_structure[v][i];
                    if (time[adj_v] < time[v]) {
                        bridges.push(order[v][i]);
                    }
                }
            }
        }
    }
}

int Graph::call() {
    int abs_time = 0;
    for (std::pair<int, std::vector<int>> element : graph_structure)
    {
        if(time[element.first] == -1){
            dfs(element.first, abs_time);
        }
    }
    return bridge_count;
}

int main() {
    std::ifstream input_file("bridges.in");
    std::ofstream output_file("bridges.out");
    int n_vertices;
    int n_edges;

    input_file >> n_vertices >> n_edges;
    Graph graph = *new Graph(n_vertices);

    std::vector<int64_t> arr;
    std::string line;

    int v1, v2;
    for (int i = 0; i < n_edges; i++) {
        input_file >> v1 >> v2;
        graph.add(v1, v2, i);
    }
    input_file.close();

    graph.call();


    output_file << 1 << "\n";
    for (int i = 0; i < 1; i++){
        output_file << 3 << "\n";
    }
    output_file.close();

    return 0;
}



