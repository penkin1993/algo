#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <deque>

struct Node {
    std::map<char, std::shared_ptr<Node>> go;
    std::shared_ptr<Node> pw; // TODO change weak_ptr
    bool is_terminal = false;
    int node_num = 0;
};

class Trie {
public:
    Trie();

    Trie(const Trie &) = delete;

    Trie(Trie &&) = delete;

    Trie &operator=(const Trie &) = delete;

    Trie &operator=(Trie &&) = delete;

    ~Trie() = default;

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

    void Print() const;

    void DefLink();

private:
    int counter = 0;
    std::shared_ptr<Node> root;

    static void print(const std::shared_ptr<Node> &node, const std::string &current);

    // Возвращает пару bool: первый - о наличии строки
    // второй - о единственности узла и необходимости его удалить.
    static std::pair<bool, bool> remove(
            std::shared_ptr<Node> &node, const std::string &key, int current_index);

    void defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque);
};

Trie::Trie() {
    root = std::make_shared<Node>();
    //root->pw = root->pw.lock();
    root->pw = root;
}

bool Trie::Has(const std::string &key) const {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) return false;
        current = next->second;
    }
    return current->is_terminal;
}

bool Trie::Add(const std::string &key) {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) {
            current = current->go[symbol] = std::make_shared<Node>();
            //current->pw = current->pw.lock(); // Также добавляем ссылку на root
            current->pw = root;
            counter++;
            //std::cout << symbol << " " << counter << "\n";
            current->node_num = counter;

        } else {
            current = next->second;
        }
    }
    // Если терминальная, значит, строка уже есть.
    if (current->is_terminal) return false;
    current->is_terminal = true;
    return true;
}

bool Trie::Remove(const std::string &key) {
    return remove(root, key, 0).first;
}

std::pair<bool, bool> Trie::remove(
        std::shared_ptr<Node> &node, const std::string &key, int current_index) {
    if (current_index == key.length()) {
        if (!node->is_terminal) return std::make_pair(false, false);
        node->is_terminal = false;
        return std::make_pair(true, node->go.empty());
    }

    auto next = node->go.find(key[current_index]);
    if (next == node->go.end()) {
        return std::make_pair(false, false);
    }

    auto result = remove(next->second, key, current_index + 1);

    if (!result.first) { // Не нашли
        return result;
    }
    if (!result.second) { // Не нужно удалить ссылку на дочерний
        return result;
    }
    node->go.erase(key[current_index]);
    return std::make_pair(true, !node->is_terminal && node->go.empty());
}

void Trie::Print() const {
    print(root, "");
}

void Trie::print(const std::shared_ptr<Node> &node, const std::string &current) {

    if (node->is_terminal) {
        std::cout << current << std::endl;
    }
    for (const auto go : node->go) {
        print(go.second, current + go.first);
    }
}

void list_fill(std::vector<std::string> &word_dict, std::string &str) {
    bool new_symbol = false;
    std::string current_string;

    for (char symbol : str) {
        if (symbol == '?') {
            new_symbol = true;
        } else if ((new_symbol) && (!current_string.empty())) {
            word_dict.push_back(current_string);
            current_string = "";
        }
        if (symbol != '?') {
            current_string += symbol;
            new_symbol = false;
        }
    }

    if (!current_string.empty()) {
        word_dict.push_back(current_string);
    }
    //for (auto & i : word_dict){ // ??adasda??sadasd??
    //    std::cout << i << "\n";
    //}
}

void Trie::DefLink() {
    std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> root_deque;

    for (const auto& iter : root->go) // добавить в очередь детей и их потомков, поскольку потомки первых детей имею ссылки на корень!!!
    {
        for (const auto& iter_child: iter.second->go){
            root_deque.push_front(std::make_tuple(iter.second, iter_child.first, iter_child.second)); // обход дочерних вершин
        }
    }
    defLink(root_deque);

    /*
    for (const auto& iter : root->go)
    {
        for (const auto& iter_child: iter.second->go){
            std::cout << iter_child.first << "\n";
        }
    }
     */
}

void Trie::defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque) {
    while (!root_deque.empty()) {
        auto element = root_deque.back();
        root_deque.pop_back();

        std::shared_ptr<Node> root_ = std::get<0>(element);
        char symbol = std::get<1>(element);
        std::shared_ptr<Node> current_ = std::get<2>(element);

        assert(root_ != nullptr); // Проверить, что вершина, на которую ссылкается не нулевая
        assert(current_ != nullptr); // Проверить, что вершина, на которую ссылкается не нулевая

        std::shared_ptr<Node>ref = root_->pw; // TODO .lock();

        //std::cout << root_->pw->node_num << " ";
        //std::cout << root_->node_num << " ";
        //std::cout << symbol << " ";
        //std::cout << current_->pw->node_num << " ";
        //std::cout << current_->node_num << "\n";

        do {
            if (ref->go.count(symbol)){ // Если существует путь по нужному ребру
                current_->pw = ref->go[symbol];
                ref = root;
            }
            else{
               ref = ref->pw; // TODO .lock();
                if (ref->go.count(symbol)) { // Если существует путь по нужному ребру
                    current_->pw = ref->go[symbol];
                    ref = root;
                }
            }
        } while (ref != root); // Пока не дойдем до корня


        //std::cout << root_->pw->node_num << " ";
        //std::cout << root_->node_num << " ";
        //std::cout << symbol << " ";
        //std::cout << current_->pw->node_num << " ";
        //std::cout << current_->node_num << "\n  \n";


        for (const auto &iter : current_->go) // добавить в очередь детей !!!
        {
            root_deque.push_front(std::make_tuple(current_, iter.first, iter.second)); // обход дочерних вершин
        }
    }
}

// TODO: Изменить нумерацию по словам !!!!

// TODO: Ввести поле состояния

// TODO: Ввести функцию перехода (при чтении очедерной буквы) !!!

// TODO: Нумерация по встречаемости в тексте

// TODO: Вернуть weak_ptr !!!!

// 3. Алгоритм обхода. 60
// 4. Финальный алгоритм. 60


// TODO : 0. Продумать весь алгоритм (решение задачи + алгоритм поиска !!!)
// TODO : 3. Релизовать второй этап !!!




int main() {
    Trie trie;
    // trie.Add("aaa");
    // trie.Add("aa");
    // trie.Remove("aaa");
    // std::cout << trie.Has("aaaa") << "\n";

    // abdk?abchijn?chnit?ijabdf?ijaij

    std::string str;
    getline(std::cin, str);

    std::vector<std::string> words_list;
    list_fill(words_list, str);

    while (!words_list.empty()) {
        trie.Add(words_list.back());
        words_list.pop_back();
    }

    trie.DefLink();
    trie.Print();


    return 0;
}



