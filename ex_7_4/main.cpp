/*
Порядковые статистики. Дано число N и N строк.
Каждая строка содержит команду добавления или удаления натуральных чисел,
а также запрос на получение k-ой порядковой статистики.
Команда добавления числа A задается положительным числом A,
команда удаления числа A задается отрицательным числом “-A”.
Запрос на получение k-ой порядковой статистики задается числом k.

Вариант 7_4.  Требуемая скорость выполнения запроса - O(log n) амортизировано.
В реализации используйте сплей дерево.
 */

#include <iostream>
#include <stack>
#include <deque>

// Узел бинарного дерева
struct TreeNode {
    int value = 0;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

    TreeNode(int _value, TreeNode *_parent) {
        value = _value;
    }
};

class Tree {
public:
    ~Tree();
    void Add(int value); // Добавление значения в дерево
private:
    TreeNode *root = nullptr;
};

Tree::~Tree() {
    // TODO:
}

void Tree::Add(int value) {
    TreeNode *local_node = root;
    if (!local_node) {
        root = new TreeNode(value, nullptr);
        return;
    }
    while (true) {
        if (value < local_node->value) {
            if (!(local_node->left)) {
                local_node->left = new TreeNode(value, local_node);
                return;
            }
            local_node = local_node->left;
        } else {
            if (!(local_node->right)) {
                local_node->right = new TreeNode(value, local_node);
                return;
            }
            local_node = local_node->right;
        }
    }
}

/*
void RightTurn(Node *&parent) {
    Node *node = parent;
    parent = node->Left;
    node->Left = parent->Right;
    parent->Right = node;
}

void LeftTurn(Node *&parent) {
    Node *node = parent;
    parent = node->Right;
    node->Right = parent->Left;
    parent->Left = node;
}

void BigRightTurn(Node *&parent) {
    LeftTurn(parent->Left);
    RightTurn(parent);
}

void BigLeftTurn(Node *&parent) {
    RightTurn(parent->Right);
    LeftTurn(parent);
}

void ZigZig(Node *&parent) {
    RightTurn(parent);
    RightTurn(parent);
}

void ZagZag(Node *&parent) {
    LeftTurn(parent);
    LeftTurn(parent);
}

void ZigZag(Node *&parent) {
    RightTurn(parent->Left);
    LeftTurn(parent);
}

void print(const std::string &prefix, const Node *node) {
    if (!node) {
        return;
    }
    std::cout << prefix + node->Key << std::endl;
    print(prefix + "-", node->Left);
    print(prefix + "-", node->Right);
}

void remove(Node *node) {
    if (!node) return;
    remove(node->Left);
    remove(node->Right);
    delete node;
}
*/
int main() {
    int N = 0;
    int64_t A = 0;
    int k = 0;
    std::cin >> N;
    Tree tree;

    for (int i = 0; i < N; i++) {
        std::cin >> A >> k;
        if (A < 0) {
            tree.Remove(-A);
        } else {
            tree.Add(A);
        }
        std::cout << tree.GetStat(k) << " ";
    }

    // TODO: Поиск (скопировать из предыдущей)
    // TODO: Поднятие элемента
    // TODO: Добавление. Изменять порядковую статистику
    // TODO: Как найти порядковую статистику. Хранить два значения
/*
    Node *node = new Node('x');
    node->Left = new Node('y');
    node->Left->Left = new Node('z');
    node->Left->Left->Left = new Node('1');
    node->Left->Left->Right = new Node('2');
    node->Left->Right = new Node('3');
    node->Right = new Node('4');

    print("", node);

    RightTurn(node);

    print("", node);

    LeftTurn(node);

    print("", node);

    ZigZig(node);

    print("", node);

    remove(node);
    */
    return 0;
}