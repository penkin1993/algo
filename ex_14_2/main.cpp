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
#include <vector>

struct Edge {
    int left;
    int right;
    int weight;

    Edge(int left_, int right_, int weight_) : left(left_), right(right_), weight(weight_) {}
};

struct CompareEdge {
    bool operator()(Edge *const &e1, Edge *const &e2) {
        return e1->weight > e2->weight;
    }
};

class EdgeQueue {

public:
    void add(int left_, int right_, int weight_);

    //~EdgeQueue();
    std::priority_queue<Edge *, std::vector<Edge *>, CompareEdge> *get();

private:
    std::priority_queue<Edge *, std::vector<Edge *>, CompareEdge> q;
};

void EdgeQueue::add(int left_, int right_, int weight_) {
    q.push(new Edge(left_, right_, weight_));
}

std::priority_queue<Edge *, std::vector<Edge *>, CompareEdge> *EdgeQueue::get() {
    return &q;
}

class DSU {
public:
    explicit DSU(size_t size);

    int find(int element);

    void merge(int left, int right);

    int get(EdgeQueue &edgeQueue);

private:
    int weight = 0;
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(size_t size) :
        parent(size, -1),
        rank(size, 1) {}

int DSU::find(int element) {
    if (parent[element] == -1) {
        return element;
    }
    return parent[element] = find(parent[element]);
}

void DSU::merge(int deputat_left, int deputat_right) {
    if (rank[deputat_left] == rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
        ++rank[deputat_left];
    } else if (rank[deputat_left] > rank[deputat_right]) {
        parent[deputat_right] = deputat_left;
    } else {
        parent[deputat_left] = deputat_right;
    }
}

int DSU::get(EdgeQueue &edgeQueue) {
    Edge *edge;
    std::priority_queue<Edge *, std::vector<Edge *>, CompareEdge> *q;
    q = edgeQueue.get();
    while (!q->empty()) {
        edge = q->top();
        q->pop();

        int deputat_left = find(edge->left - 1);
        int deputat_right = find(edge->right - 1);

        if (deputat_left != deputat_right) {
            merge(deputat_left, deputat_right);
            weight += edge->weight;
        }
    }
    return weight;
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

    DSU spanTree = DSU(n_vertices);
    int weight = spanTree.get(edgeQueue);

    std::cout << weight;
    return 0;
}






