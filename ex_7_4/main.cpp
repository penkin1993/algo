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
    explicit TreeNode(int64_t _value, TreeNode *_parent) :
            value(_value),
            parent(_parent) {}

    int64_t value = 0;
    TreeNode *parent = nullptr;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
};

class Tree {
public:
    ~Tree();

    void Add(int64_t value);

    void Splay(TreeNode *node);

    void Split();

    void Remove();

    void GetStat();

private:
    TreeNode *root = nullptr;

    void Zig(TreeNode *root);

    void Zag(TreeNode *root);

    void ZigZig(TreeNode *root);

    void ZagZag(TreeNode *root);

    void ZigZag(TreeNode *root);
};

Tree::~Tree() {
    // TODO:
}

void Tree::Add(int64_t value) {
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

void Tree::Zig(TreeNode *root) {
    TreeNode *node = root;
    root = node->left;
    node->left = root->right;
    root->right = node;
}

void Tree::Zag(TreeNode *root) {
    TreeNode *node = root;
    root = node->right;
    node->right = root->left;
    root->left = node;
}

void Tree::ZigZig(TreeNode *root) {
    Zig(root);
    Zig(root);
}

void Tree::ZagZag(TreeNode *root) {
    Zag(root);
    Zag(root);
}

void Tree::ZigZag(TreeNode *root) {
    Zig(root->left);
    Zag(root);
}

/*
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