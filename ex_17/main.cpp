#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <deque>
#include <unordered_map>

struct Node {
    std::map<char, std::shared_ptr<Node>> go;
    std::shared_ptr<Node> pw; // TODO change weak_ptr
    bool is_terminal = false;
    std::vector<int> word_num; // id терминальных врешин
    std::unordered_map<char, std::shared_ptr<Node>> cash_pw;
    int node_num; // TODO temp
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

    std::vector<int> Step(char symbol); // основной цикл алгоритма

private:
    int counter = 0;
    int counter_node_num = 0;


    std::shared_ptr<Node> root;
    std::shared_ptr<Node> current_state; // состояние автомата

    static void print(const std::shared_ptr<Node> &node, const std::string &current);

    // Возвращает пару bool: первый - о наличии строки
    // второй - о единственности узла и необходимости его удалить.
    static std::pair<bool, bool> remove(
            std::shared_ptr<Node> &node, const std::string &key, int current_index);

    void defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque);

    bool step_down(std::vector<int> & symbols_id, char symbol); // вспомогательная функция основного алгоритма
    bool step_link(std::vector<int> & symbols_id, char symbol); // вспомогательная функция прохода по ссылкам
};

Trie::Trie() {
    root = std::make_shared<Node>();
    //root->pw = root->pw.lock();
    root->pw = root;
    current_state = root;
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
            counter_node_num++; // TODO temp
            current->node_num = counter_node_num; // TODO temp
            //std::cout << symbol << " " << counter << "\n";
        } else {
            current = next->second;
        }
    }
    // Если терминальная, значит, строка уже есть.
    if (current->is_terminal) return false;
    current->is_terminal = true;
    counter++;
    current->word_num.push_back(counter); // номер слова
    //std::cout << key << " ";
    //std::cout << current->go.size() << "\n";
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

void list_fill(std::deque<std::string> &word_dict, std::string &str) {
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

            for (int id : current_->pw->word_num){ // родительские терминальные варшины (имеют один общий суффикс)
                current_->word_num.push_back(id);
            }

        } while (ref != root); // Пока не дойдем до корня

        //std::cout << root_->node_num << " ";
        //std::cout << root_->pw->node_num << " ";
        //std::cout << current_->node_num << " ";
        //std::cout << symbol << " ";
        //for (int num : current_->word_num){
        //    std::cout << num;
        //}
        //std::cout << "\n  \n";
        //std::cout << current_->pw->node_num << "\n  \n";


        for (const auto &iter : current_->go) // добавить в очередь детей !!!
        {
            root_deque.push_front(std::make_tuple(current_, iter.first, iter.second)); // обход дочерних вершин
        }
    }
}

std::vector<int> Trie::Step(char symbol){
    bool is_finished = false;
    std::vector<int> symbols_id;

    //std::cout << current_state->node_num << " " << symbol << " ";

    std::shared_ptr<Node> old_state = current_state; // запоминаем для hash_map

    //std::cout << symbol << " ";
    //std::cout << current_state->go.size() << "\n";

    while (!is_finished) {
        //std :: cout << "!!!!!1 \n";
        //std :: cout << current_state->node_num << "\n";
        is_finished = step_down(symbols_id, symbol);
        //std :: cout << "!!!!!2 \n";
        //std::cout << is_finished  << current_state->node_num << "\n";
        //std :: cout << "!!!!!3 \n";
        //std :: cout << "\n";

        // Пытаемся пойти вниз.
        //    Если возможно, то идем
        //       Если вершина терминальная, то добавляем ее индексы в output
        if (!is_finished){
            is_finished = step_link(symbols_id, symbol); // TODO: Можно сразу пытаться перейти по длинным суффиксным ссылкам !!!
        }
        // Если нет, то переходим по суффиусным ссылкам, и пытаемся пойти вниз
        // Повторяем рекурсивно верхний цикл
        // Пытаемся пока не дошли до корня
    }
    // Запоминаем вершину перехода в cash_pw
    old_state->cash_pw[symbol] = current_state;

    //std::cout << old_state->node_num << " " << symbol << " ";
    //std::cout << current_state->node_num << "\n";

    return symbols_id;
}

bool Trie::step_down(std::vector<int> & symbols_id, char symbol){
    //std :: cout << "step_down \n";
    if (current_state->go.count(symbol)) {
        /*
        std::cout << " \n DOWN \n";
        for (auto it = current_state->go.cbegin(); it != current_state->go.cend(); ++it)
        {
            std::cout << it->first << "\n";
        }
        std::cout << "DOWN \n \n";
        */

        current_state = current_state->go[symbol];
        for (int id : current_state->word_num){ // перешли в новое состояние и запушили id слов
            //std::cout << " push_down \n";
            symbols_id.push_back(id);
        }
        return true;
    }
    return false;
}

bool Trie::step_link(std::vector<int> & symbols_id, char symbol) {
    if (current_state->cash_pw.count(symbol)){ // если есть длнная ссылка в кэше
        current_state = current_state->cash_pw[symbol];
        for (int id : current_state->word_num){ // перешли в новое состояние и запушили id слов
            //std::cout << " push_link \n";
            symbols_id.push_back(id);
        }
       // std::cout << "long_link" << "\n";
        return true; // прошли по длинной суффиксной ссылке
    } else {
        current_state = current_state->pw; // прошли по суффиксной ссылке
        if (current_state != root){
            return false;
        } else {
            step_down(symbols_id, symbol);
            return true;
        }
    }
}



// TODO: 3. Шаг 2. Подсчет и финальные позиции


// TODO: Вернуть weak_ptr !!!!


// TODO : 0. Продумать весь алгоритм (решение задачи + алгоритм поиска !!!)


int main() {
    Trie trie;
    // trie.Add("aaa");
    // trie.Add("aa");
    // trie.Remove("aaa");
    // std::cout << trie.Has("aaaa") << "\n";

    // abdk?abchijn?chnit?ijabdf?ijaij
    // c?bc?abc?

    std::string str;
    getline(std::cin, str);

    std::deque<std::string> words_list;
    list_fill(words_list, str);

    while (!words_list.empty()) {
        trie.Add(words_list.back()); // TODO: front
        words_list.pop_back(); // TODO: pop_front
    }

    trie.DefLink();
    //trie.Print();

    std::vector<int> out;
    char symbol = ' ';

    while (symbol != '\n') //Считывание в массив подстроки
    {
        std::cin.get(symbol);
        out = trie.Step(symbol); // TODO: Print на один больше !!!
        std::cout << out.size();
    }
    return 0;
}
/*
ab??aba
ababacaba

aa??bab?cbaa?
aabbbabbcbaabaabbbabbcbaab
 */



