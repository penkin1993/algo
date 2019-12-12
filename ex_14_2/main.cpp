/*
B. Минимальное остовное дерево 2018
Ограничение времени	0.2 секунды
Ограничение памяти	10Mb
Ввод	стандартный ввод или kruskal.in
Вывод	стандартный вывод или kruskal.out
Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.
Вариант 2. С помощью алгоритма Крускала.
Вариант 3. С помощью алгоритма Борувки.
Ваш номер варианта прописан в ведомости.
Формат ввода
Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
Следующие m строк содержат описание ребер по одному на строке.
Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).

Формат вывода
Выведите единственное целое число - вес минимального остовного дерева.

Пример 1
Ввод	Вывод
4 4
1 2 1
2 3 2
3 4 5
4 1 4
7
Пример 2
Ввод	Вывод
5 10
4 3 3046
4 5 90110
5 1 57786
3 2 28280
4 3 18010
4 5 61367
4 1 18811
4 2 69898
3 5 72518
3 1 85838
107923
*/

#include <iostream>
#include <queue>
#include <unordered_map>

struct Edge {
    int left;
    int right;
    int weight;

    Edge(int left_, int right_, int weight_) : left(left_), right(right_), weight(weight_) {}
    ~Edge(){
        //delete Edge; // TODO: ????
    }
};

struct CompareEdge {
    bool operator()(Edge *const &e1, Edge *const &e2) {
        return e1->weight > e2->weight;
    }
};

class EdgeQueue {

public:
    void add(int left_, int right_, int weight_);
    ~EdgeQueue();
    Edge * get();

private:
    std::priority_queue<Edge *, std::vector<Edge *>, CompareEdge> q;
};

void EdgeQueue::add(int left_, int right_, int weight_) {
    q.push(new Edge(left_, right_, weight_));
}

Edge * EdgeQueue::get() {
    if (!q.empty()) {
        Edge * e = q.top();
        q.pop(); // TODO: Отрабатывает ли деструктор ???
        return e;
    }
    else{
        return nullptr;
    }
}

EdgeQueue::~EdgeQueue() { // TODO: Дестркутор !!!

}

/////////////////////////////////////////////////////////////////////////
class SpanTree {
public:
    explicit SpanTree(int n_vertices);
    ~SpanTree(); // TODO !!!???
    int get(EdgeQueue & edgeQueue); // основная бизнес логика
private:
    int64_t weight = 0; // вес минимального остовного дерева
    std::unordered_map<int, int> vertices;
    void uniteSets(int component1, int component2, int weight_);
    int findComponent(int vert);
};

SpanTree::SpanTree(int n_vertices) {
    for (int i = 1; i < n_vertices + 1; i++){
        vertices[i] = i;
    }
}

SpanTree::~SpanTree() {}

int SpanTree::get(EdgeQueue & edgeQueue) {
    Edge * edge;
    int component1;
    int component2;
    while( (edge = edgeQueue.get()) != nullptr){
        component1 = findComponent(edge->left);
        component2 = findComponent(edge->right);
        if (component1 != component2){
            uniteSets(component1, component2, edge->weight); // левое множество, правое множество, новый вес
        }
    }
    return weight;
}

void SpanTree::uniteSets(int component1, int component2, int weight_) {
    ////////////////////////////////////////////////////////////////////
    int currentVert = component2;
    int nextVert = component2;
    while (nextVert != vertices[nextVert]) {
        nextVert = vertices[currentVert]; // переходим к следующему элементу
        vertices[currentVert] = component1; // присваиваем вершине
        currentVert = nextVert;
    }
    //////////////////////////////////////////////////////////////////// 
    vertices[component2] = component1;
    weight += weight_;
}

/*
int SpanTree::findComponent(int vert, bool isFirst) {
    int currentVert = vert;
    int nextVert = vert;
    while (nextVert != vertices[nextVert]){
        nextVert = vertices[currentVert]; // переходим к следующему элементу
        //if (isFirst) {
        vertices[currentVert] = vert; // присваиваем вершине
        currentVert = nextVert;
        //}
    }
    vertices[nextVert] = vert; // меняеем рута кмопоннеты

    if (isFirst) {
        return vert;
    }
    else{
        return nextVert;
    }
}
*/

int SpanTree::findComponent(int vert) {
    int currentVert = vert;
    while (currentVert != vertices[currentVert]){
        currentVert = vertices[currentVert]; // переходим к следующему элементу
    }
    vertices[vert] = currentVert;
    return currentVert;
}



int main() {
    int n_vertices;
    int n_edges;

    std::cin >> n_vertices >> n_edges;
    EdgeQueue edgeQueue;

    int left_;
    int right_;
    int weight_;

    for (int i = 0; i < n_edges; i++) {
        std::cin >> left_ >> right_ >> weight_;
        edgeQueue.add(left_, right_, weight_);
    }

    SpanTree spanTree = SpanTree(n_vertices);
    int64_t weight = spanTree.get(edgeQueue);

    std::cout << weight;
    return 0;
}