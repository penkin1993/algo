/*
Задача 6. Порядок обхода (3 балла)
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root,
если root→Key ≤ K, то узел K добавляется в правое поддерево root;
иначе в левое поддерево root.
Рекурсия запрещена.


 6_2. Выведите элементы в порядке in-order (слева направо).
in
out
3
2 1 3
1 2 3
3
1 2 3
1 2 3
3
3 1 2
1 2 3

 */

#include <iostream>
#include <stack>
#include <deque>

// Узел бинарного дерева
struct TreeNode {
    explicit TreeNode(int _value) : value(_value) {}
    int value = 0;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
};

class Tree {
public:
    ~Tree();
    void Print(); // Печать всех значений
    void Add(int value); // Добавление значения в дерево
private:
    std::deque<TreeNode *> GetDeque(); // Иттератор для обхода
    TreeNode *root = nullptr;
};

Tree::~Tree() {
    std::deque<TreeNode *> final_deque = GetDeque();
    while (!final_deque.empty()) {
        delete final_deque.front();
        final_deque.pop_front();
    }
}

void Tree::Print() {
    std::deque<TreeNode *> final_deque = GetDeque();
    while (!final_deque.empty()) {
        std::cout << final_deque.front()->value << " ";
        final_deque.pop_front();
    }
}

void Tree::Add(int value) {
    TreeNode *local_node = root;
    if (!local_node) {
        root = new TreeNode(value);
        return;
    }
    while (true) {
        if (value < local_node->value) {
            if (!(local_node->left)) {
                local_node->left = new TreeNode(value);
                return;
            }
            local_node = local_node->left;
        } else {
            if (!(local_node->right)) {
                local_node->right = new TreeNode(value);
                return;
            }
            local_node = local_node->right;
        }
    }
}


std::deque<TreeNode *> Tree::GetDeque() {
    std::deque<TreeNode *> final_deque;
    if (!root) {
        return final_deque;
    }
    TreeNode *local_node = root;
    std::stack<TreeNode *> local_stack;

    while ((local_node != nullptr) || (!local_stack.empty())) {
        if (local_node != nullptr) {
            local_stack.push(local_node);
            local_node = local_node->left;
        } else if (!local_stack.empty()) {
            local_node = local_stack.top();
            local_stack.pop();
            final_deque.push_back(local_node);
            local_node = local_node->right;
        }
    }
    return final_deque;
}


int main() {
    int N = 0;
    std::cin >> N;
    int64_t value = 0;

    Tree tree;
    for (int i = 0; i < N; ++i) {
        std::cin >> value;
        tree.Add(value);
    }

    tree.Print();
    return 0;
}