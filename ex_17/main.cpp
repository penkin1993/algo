#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <unordered_map>


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
    int current_state = 0; // состояние автомата
    std::vector<int> path_nodes;
    std::vector<int> node_array; // массив со всеми нодами
    std::vector<int> parent_id; // индекс суффиксной вершины
    std::vector<std::vector<int>> word_num;
    std::vector<std::unordered_map<char, int>> go;
    std::vector<std::unordered_map<char, int>> cash_pw;

    void defLink(std::deque<std::tuple<int, int, char>> &root_deque);

    bool step_down(std::deque<int> &symbols_id, char symbol); // вспомогательная функция основного алгоритма
    bool step_long_link(std::deque<int> &symbols_id, char symbol); // вспомогательная функция прохода по ссылкам
    bool step_short_link(std::deque<int> &symbols_id, char symbol,
                         std::vector<int> &path_nodes); // вспомогательная функция прохода по ссылкам
};

Trie::Trie() {
    node_array.push_back(0);
    parent_id.push_back(0);
    word_num.emplace_back();
    go.emplace_back();
    cash_pw.emplace_back();
}


bool Trie::Add(const std::string &key, int id_terminal) {
    int current = 0;
    for (char symbol : key) {
        auto next = go[current].find(symbol);
        if (next == go[current].end()) {
            counter_id++;
            parent_id.push_back(0); // суффиксная ссылка вначале указывает на родителя
            node_array.push_back(counter_id); // добавили в массив
            go.emplace_back(); // добавить новый словарь
            go[current].insert(std::pair<char, int>(symbol, counter_id));// добавить у родителя ссыокку в go
            cash_pw.emplace_back();
            word_num.emplace_back();
            current = counter_id; // перейти к сыну
        } else {
            current = next->second;
        }
    }
    word_num[current].push_back(id_terminal); // номер слова
    return true;
}

void Trie::DefLink() {
    std::deque<std::tuple<int, int, char>> root_deque;

    for (const auto &iter : go[0]) // добавить в очередь детей и их потомков, поскольку потомки первых детей имею ссылки на корень!!!
    {
        for (const auto &iter_child: go[iter.second]) {
            root_deque.push_front(
                    std::make_tuple(iter.second, iter_child.second, iter_child.first)); // обход дочерних вершин
        }
    }
    defLink(root_deque);
}

void Trie::defLink(std::deque<std::tuple<int, int, char>> &root_deque) {
    while (!root_deque.empty()) {
        auto element = root_deque.back();
        root_deque.pop_back();

        int root_ = std::get<0>(element);
        int current_ = std::get<1>(element);
        char symbol = std::get<2>(element);
        int ref = parent_id[root_];

        while (true) {
            if (go[ref].count(symbol)) { // Если существует путь по нужному ребру
                parent_id[current_] = go[ref].at(symbol);
                for (int id : word_num[parent_id[current_]]) { // родительские терминальные варшины (имеют один общий суффикс)
                    word_num[current_].push_back(id);
                }
                break;
            } else if (ref == 0){
                break;
            }
            ref = parent_id[ref];
        }

        for (const auto &iter : go[current_]) // добавить в очередь детей !!!
        {
            root_deque.push_front(std::make_tuple(current_, iter.second, iter.first)); // обход дочерних вершин
        }
    }
}




std::deque<int> Trie::Step(char symbol) {
    bool is_finished = false;
    std::deque<int> symbols_id;
    // Создаем буфер из вершин которые посетили и сразу во всех обновляем словрь длинных суффиксных ссылкок
    path_nodes.push_back(current_state);

    while (!is_finished) {
        is_finished = step_down(symbols_id, symbol);
        if (is_finished) {
            break;
        }
        is_finished = step_long_link(symbols_id, symbol);
        if (is_finished) {
            break;
        }
        is_finished = step_short_link(symbols_id, symbol, path_nodes);
    }
    // Запоминаем вершину перехода в cash_pw
    while (!path_nodes.empty()) {
        symbol = path_nodes.back();
        //if (!cash_pw[symbol].count(symbol)){
        cash_pw[symbol].insert(std::pair<char, int>(symbol, current_state));
        //}
        path_nodes.pop_back();
    }
    return symbols_id;
}


bool Trie::step_down(std::deque<int> &symbols_id, char symbol) {
    if (go[current_state].count(symbol)) {
        current_state = go[current_state].at(symbol);
        for (int id : word_num[current_state]) { // перешли в новое состояние и запушили id слов
            symbols_id.push_back(id);
        }
        return true;
    }
    return false;
}

bool Trie::step_long_link(std::deque<int> &symbols_id, char symbol) {
    if (cash_pw[current_state].count(symbol)) { // если есть длнная ссылка в кэше
        current_state = cash_pw[current_state].at(symbol);
        for (int id : word_num[current_state]) { // перешли в новое состояние и запушили id слов
            symbols_id.push_back(id);
        }
        return true; // прошли по длинной суффиксной ссылке
    }
    return false;
}


bool Trie::step_short_link(std::deque<int> &symbols_id, char symbol, std::vector<int> &path_nodes) {
    current_state = parent_id[current_state]; // прошли по суффиксной ссылке
    if (current_state != 0) {
        path_nodes.push_back(current_state);
        return false;
    } else {
        step_down(symbols_id, symbol);
        return true;
    }
}

void list_fill(std::deque<std::string> &word_dict, std::deque<int> &shifts_, std::string &str) {
    bool new_symbol = false;
    std::string current_string;
    int counter = -1;

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
    Pattern(std::deque<int> &shifts_, int state_size_, int text_length_) :
            shifts(shifts_), state_size(state_size_), text_length(text_length_){
        words_count = shifts_.size();
        state = new int[state_size_ + text_length_]{0};
        counter = state_size_;
    };

    Pattern(const Pattern &) = delete;

    Pattern(Pattern &&) = default;

    Pattern &operator=(const Pattern &) = delete;

    Pattern &operator=(Pattern &&) = delete;

    ~Pattern();

    void Step(std::deque<int> &symbols_id);

private:
    int * state;
    int counter;
    int words_count; // сколько всего слов
    int text_length;
    const int state_size; // длина входной строки веместе со всеми ???
    const std::deque<int> &shifts; // на сколько нужно сдвигать
};

Pattern::~Pattern() {
    delete [] state;
}

void Pattern::Step(std::deque<int> &symbols_id) {
    for (int id : symbols_id){
        state[counter - shifts[id]]++; // Отсчет будет не сразу
    }

    if (state[counter - state_size + 1] == words_count){
        if (counter - state_size - state_size + 1 >= 0) {
            std::cout << counter - state_size - state_size + 1 << " ";
        }
    }
    counter++;
}


int main() {
    std::iostream::sync_with_stdio(false);
    Trie trie;
    std::string str;
    std::cin >> str;
    int state_size_ = str.length();

    std::deque<std::string> words_list;
    std::deque<int> shifts_;
    list_fill(words_list, shifts_, str);

    for (int i = 0; i < shifts_.size(); i++) {
        trie.Add(words_list.front(), i);
        words_list.pop_front();
    }
    trie.DefLink();

    std::deque<int> out;

    std::string text;
    std::cin >> text;

    Pattern pattern = Pattern(shifts_, state_size_, text.length());

    for (char & it : text) {
        out = trie.Step(it);
        pattern.Step(out);
    }
    return 0;
}

/*

aa??bab?cbaa?
aabbbabbcbaabaabbbabbcbaab

?a?a?a?a?a?a?a?a?a?a????????
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


??ab??ab??
ababacabaabababababababa

???
aaa

ab??aba
ababacaba

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



