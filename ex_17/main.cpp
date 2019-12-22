#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <deque>

struct Node {
    std::map<char, std::shared_ptr<Node>> go;
    std::weak_ptr<Node> pw;
    bool is_terminal = false;
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
    std::shared_ptr<Node> root;

    static void print(const std::shared_ptr<Node> &node, const std::string &current);

    // Возвращает пару bool: первый - о наличии строки
    // второй - о единственности узла и необходимости его удалить.
    static std::pair<bool, bool> remove(
            std::shared_ptr<Node> &node, const std::string &key, int current_index);

    static void defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque);
};

Trie::Trie() {
    root = std::make_shared<Node>();
    std::shared_ptr<Node> pw = root->pw.lock();
    pw = root;
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
            std::shared_ptr<Node> pw = current->pw.lock(); // Также добавляем ссылку на root
            pw = root;
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
    // TODO : Print Указатели pw


}

void Trie::defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque) {
    if (!root_deque.empty()) {
        auto element = root_deque.back();
        root_deque.pop_back();

        std::shared_ptr<Node> root_ = std::get<0>(element);
        char symbol = std::get<1>(element);
        std::shared_ptr<Node> current_ = std::get<2>(element);

        assert(root_ != nullptr); // Проверить, что вершина, на которую ссылкается не нулевая
        assert(current_ != nullptr); // Проверить, что вершина, на которую ссылкается не нулевая

        if (root_->go.count(symbol)) { // Проверить есть ли в go char
            current_->pw = root_->go[symbol];// Если есть, то меняем ее
            //    }
        }
        for (const auto &iter : current_->go) // добавить в очередь детей !!!
        {
            root_deque.push_front(std::make_tuple(current_, iter.first, iter.second)); // обход дочерних вершин
        }
    }
}



// TODO : Ввести поле состояния

// TODO : Ввести функцию перехода (при чтении очедерной буквы) !!!





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


// 3. Алгоритм обхода. 60
// 4. Финальный алгоритм. 60


// TODO : 0. Продумать весь алгоритм (решение задачи + алгоритм поиска !!!)
// TODO : 3. Релизовать второй этап !!!


