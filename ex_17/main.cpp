#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <unordered_map>


struct Node {
    Node(int id_) : id(id_){}
    int id;
    std::unordered_map<char, int> go; // переход в id вершины
    bool is_terminal = false;
    std::vector<int> word_num; // id терминальных врешин
    std::unordered_map<char, int> cash_pw;
};

class Trie {
public:
    Trie();

    Trie(const Trie &) = delete;

    Trie(Trie &&) = delete;

    Trie &operator=(const Trie &) = delete;

    Trie &operator=(Trie &&) = delete;

    ~Trie() = default;

    bool Add(const std::string &key, int id);

    void DefLink();

    std::deque<int> Step(char symbol); // основной цикл алгоритма

private:
    int counter_id = 0;
    std::vector<std::shared_ptr<Node>> node_array; // массив со всеми нодами
    std::vector<int> parent_id; // индекс суффиксной вершины
    int current_state = 0; // состояние автомата

    // Возвращает пару bool: первый - о наличии строки
    // второй - о единственности узла и необходимости его удалить.

    void defLink(std::deque<std::tuple<int, char, int>> &root_deque);

    bool step_down(std::deque<int> &symbols_id, char symbol); // вспомогательная функция основного алгоритма
    bool step_long_link(std::deque<int> &symbols_id, char symbol); // вспомогательная функция прохода по ссылкам
    bool step_short_link(std::deque<int> &symbols_id, char symbol, std::vector<int> &path_nodes); // вспомогательная функция прохода по ссылкам
};

Trie::Trie() {
    node_array.push_back(std::make_shared<Node>(0));
    parent_id.push_back(0);
}


bool Trie::Add(const std::string &key, int id_terminal) {
    int current = 0;
    for (char symbol : key) {
        auto next = node_array[current]->go.find(symbol);
        if (next == node_array[current]->go.end()) {
            counter_id++;
            parent_id.push_back(0); // суффиксная ссылка вначале указывает на родителя
            node_array.push_back(std::make_shared<Node>(counter_id)); // добавили в массив
            node_array[current]->go[symbol] = counter_id; // добавить у родителя ссыокку в go
            current = counter_id; // перейти к сыну
            //std::cout << symbol << " " << counter << "\n";
        } else {
            current = next->second;
        }
    }
    node_array[current]->word_num.push_back(id_terminal); // номер слова
    return true;
}

void Trie::DefLink() {
    std::deque<std::tuple<int, char, int>> root_deque;

    for (const auto &iter : node_array[0]->go) // добавить в очередь детей и их потомков, поскольку потомки первых детей имею ссылки на корень!!!
    {
        for (const auto &iter_child: node_array[iter.second]->go) {
            root_deque.push_front(
                    std::make_tuple(iter.second, iter_child.first, iter_child.second)); // обход дочерних вершин
        }
    }
    defLink(root_deque);
}

void Trie::defLink(std::deque<std::tuple<int, char, int>> &root_deque) {
    while (!root_deque.empty()) {
        auto element = root_deque.back();
        root_deque.pop_back();

        int root_ = std::get<0>(element);
        char symbol = std::get<1>(element);
        int current_ = std::get<2>(element);
        int ref = parent_id[root_];

        do {
            if (node_array[ref]->go.count(symbol)) { // Если существует путь по нужному ребру
                parent_id[current_] = node_array[ref]->go[symbol];
                ref = 0;
            } else {
                ref = parent_id[ref]; // переходим по ссылке родителя
                if (node_array[ref]->go.count(symbol)) { // Если существует путь по нужному ребру 
                    parent_id[current_] = node_array[ref]->go[symbol];
                    ref = 0;
                }
            }
            for (int id : node_array[parent_id[current_]]->word_num) { // родительские терминальные варшины (имеют один общий суффикс) // TODO: Отрабатывает один раз ???
                node_array[current_]->word_num.push_back(id);
            }

        } while (ref != 0); // Пока не дойдем до корня


        for (const auto &iter : node_array[current_]->go) // добавить в очередь детей !!!
        {
            root_deque.push_front(std::make_tuple(current_, iter.first, iter.second)); // обход дочерних вершин
        }
    }
}



std::deque<int> Trie::Step(char symbol) {
    bool is_finished = false;
    std::deque<int> symbols_id;
    //std::shared_ptr<Node> old_state = current_state; // запоминаем для hash_map
    std::vector<int> path_nodes;
    // Создаем буфер из вершин которые посетили и сразу во всех обновляем словрь длинных суффиксных ссылкок
    path_nodes.push_back(current_state);

    while (!is_finished) {
        is_finished = step_down(symbols_id, symbol);
        if (is_finished){
            break;
        }
        is_finished = step_long_link(symbols_id, symbol);
        if (is_finished){
            break;
        }
        is_finished = step_short_link(symbols_id, symbol, path_nodes);
    }
    // Запоминаем вершину перехода в cash_pw
    //old_state->cash_pw[symbol] = current_state;
    while (!path_nodes.empty()){
        node_array[path_nodes.back()]->cash_pw[symbol] = current_state;
        path_nodes.pop_back();
    }
    return symbols_id;
}


bool Trie::step_down(std::deque<int> &symbols_id, char symbol) {
    if (node_array[current_state]->go.count(symbol)) {
        current_state = node_array[current_state]->go[symbol];
        for (int id : node_array[current_state]->word_num) { // перешли в новое состояние и запушили id слов
            symbols_id.push_back(id);
        }
        return true;
    }
    return false;
}

bool Trie::step_long_link(std::deque<int> &symbols_id, char symbol) {
    if (node_array[current_state]->cash_pw.count(symbol)) { // если есть длнная ссылка в кэше
        current_state = node_array[current_state]->cash_pw[symbol];
        for (int id : node_array[current_state]->word_num) { // перешли в новое состояние и запушили id слов
            symbols_id.push_back(id);
        }
        return true; // прошли по длинной суффиксной ссылке
    }
    return false;
}


bool Trie::step_short_link(std::deque<int> &symbols_id, char symbol, std::vector<int> &path_nodes) {
    current_state = parent_id[current_state]; // прошли по суффиксной ссылке

    if (current_state != 0) {
        return false;
    } else {
        step_down(symbols_id, symbol);
        return true;
    }
}

void list_fill(std::deque<std::string> &word_dict, std::deque<int> &shifts_, std::string &str, int &left_q) {
    bool new_symbol = false;
    std::string current_string;
    int counter = -1;
    for (char symbol : str) {
        if (symbol == '?') {
            left_q++;
        } else {
            break;
        }
    }

    int counter_q = 0;
    for (char symbol : str) {
        if (symbol == '?') {
            counter_q++;
            new_symbol = true;
        } else if ((new_symbol) && (!current_string.empty())) {
            word_dict.push_back(current_string);
            shifts_.push_back(counter - counter_q);
            current_string = "";
        }
        if (symbol != '?') {
            current_string += symbol;
            new_symbol = false;
            counter_q = 0;
        }
        counter++;
    }
    if (!current_string.empty()) {
        word_dict.push_back(current_string);
        shifts_.push_back(counter - counter_q);
    }
}

class Pattern {
public:
    Pattern(std::deque<int> &shifts_, int state_size_) :
            shifts(shifts_), state_size(state_size_) {
        words_count = shifts_.size();
        for (int i = 0; i < state_size; i++) {
            state.push_back(0);
        }
    };
    Pattern(const Pattern &) = delete;

    Pattern(Pattern &&) = default;

    Pattern &operator=(const Pattern &) = delete;

    Pattern &operator=(Pattern &&) = delete;

    ~Pattern() = default;

    void Step(std::deque<int> &symbols_id, int left_q);

    void Print(int left_q);

private:
    int counter = 1;
    int words_count; // сколько всего слов
    const int state_size; // длина входной строки веместе со всеми ???
    const std::deque<int> &shifts; // на сколько нужно сдвигать
    std::deque<int> state; // текущая очередь состояний
    std::deque<int> answer; // то, что возвращать в ответ
};

void Pattern::Step(std::deque<int> &symbols_id, const int left_q) {
    int id;

    while (!symbols_id.empty()) {
        id = symbols_id.back();
        symbols_id.pop_back();
        state[shifts[id]]++;
    }
    if (state.back() == words_count) {
        answer.push_back(counter - state_size - left_q);
    }

    state.pop_back();
    state.push_front(0);
    counter++;
}

void Pattern::Print(int left_q) {
    int ans;
    counter -= 1;
    while (!answer.empty()) {
        ans = answer.front() + left_q;
        if ((ans >= 0) && (ans + state_size <= counter)) {
            std::cout << ans << " ";
        }
        answer.pop_front();
    }
}

int main() {
    std::iostream::sync_with_stdio( false );
    Trie trie;
    std::string str;
    std::cin >> str;
    int state_size_ = str.length();

    std::deque<std::string> words_list;
    std::deque<int> shifts_;
    int left_q = 0;
    list_fill(words_list, shifts_, str, left_q);

    for (int i = 0; i < shifts_.size(); i++) {
        trie.Add(words_list.front(), i);
        words_list.pop_front();
    }
    trie.DefLink();

    std::deque<int> out;

    Pattern pattern = Pattern(shifts_, state_size_);

    std::string text;
    std::cin >> text;

    for (std::string::iterator it = text.begin(); it != text.end(); ++it) {

        out = trie.Step(*it);
        pattern.Step(out, left_q);
    }

    pattern.Print(left_q);

    return 0;
}

/*

?a?a?a?a?a?a?a?a?a?a????????
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


??ab??ab??
ababacabaabababababababa

???
aaa

ab??aba
ababacaba


aa??bab?cbaa?
aabbbabbcbaabaabbbabbcbaab


ba?aab?abab
aababab


??a
abca


a?aa?aaa??a?aa?
aaaaaaaaaaaaaaa


??b
bbbaaaaaaaaaaa


?a?aa?aaa??a?aa?
aaaaaaaaaaaaaaaa


??
asdasdsad


??asd?s??
asdasdasad


??
a


?bb?????a?????
bbbaaaaaaaaaaa


??
bbaaaa


???????a
aaaaaaaa

0

b?a
abba

*/



