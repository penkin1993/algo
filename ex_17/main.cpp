#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include <unordered_map>

struct Node {
    std::unordered_map<char, std::shared_ptr<Node>> go;
    std::shared_ptr<Node> pw; // TODO change weak_ptr
    bool is_terminal = false; // TODO: Убрать это нафиг !!!
    std::vector<int> word_num; // id терминальных врешин
    std::unordered_map<char, std::shared_ptr<Node>> cash_pw;
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

    std::vector<int> Step(char symbol); // основной цикл алгоритма

private:
    int counter_node_num = 0;

    std::shared_ptr<Node> root;
    std::shared_ptr<Node> current_state; // состояние автомата

    // Возвращает пару bool: первый - о наличии строки
    // второй - о единственности узла и необходимости его удалить.
    static std::pair<bool, bool> remove(
            std::shared_ptr<Node> &node, const std::string &key, int current_index);

    void defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque);

    bool step_down(std::vector<int> &symbols_id, char symbol); // вспомогательная функция основного алгоритма
    bool step_long_link(std::vector<int> &symbols_id, char symbol); // вспомогательная функция прохода по ссылкам
    bool step_short_link(std::vector<int> &symbols_id, char symbol); // вспомогательная функция прохода по ссылкам
};

Trie::Trie() {
    root = std::make_shared<Node>();
    //root->pw = root->pw.lock();
    root->pw = root;
    current_state = root;
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


void Trie::DefLink() {
    std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> root_deque;

    for (const auto &iter : root->go) // добавить в очередь детей и их потомков, поскольку потомки первых детей имею ссылки на корень!!!
    {
        for (const auto &iter_child: iter.second->go) {
            root_deque.push_front(
                    std::make_tuple(iter.second, iter_child.first, iter_child.second)); // обход дочерних вершин
        }
    }
    defLink(root_deque);
}

std::vector<int> Trie::Step(char symbol) {
    bool is_finished = false;
    std::vector<int> symbols_id;
    std::shared_ptr<Node> old_state = current_state; // запоминаем для hash_map
    while (!is_finished) {
        is_finished = step_down(symbols_id, symbol);
        if (is_finished){
            break;
        }
        is_finished = step_long_link(symbols_id, symbol);
        if (is_finished){
            break;
        }
        is_finished = step_short_link(symbols_id, symbol);
    }
    // Запоминаем вершину перехода в cash_pw
    old_state->cash_pw[symbol] = current_state;
    return symbols_id;
}

bool Trie::step_down(std::vector<int> &symbols_id, char symbol) {
    if (current_state->go.count(symbol)) {
        current_state = current_state->go[symbol];
        for (int id : current_state->word_num) { // перешли в новое состояние и запушили id слов
            symbols_id.push_back(id);
        }
        return true;
    }
    return false;
}

bool Trie::step_long_link(std::vector<int> &symbols_id, char symbol) {
    if (current_state->cash_pw.count(symbol)) { // если есть длнная ссылка в кэше
        current_state = current_state->cash_pw[symbol];
        for (int id : current_state->word_num) { // перешли в новое состояние и запушили id слов
            symbols_id.push_back(id);
        }
        return true; // прошли по длинной суффиксной ссылке
    }
    return false;
}

bool Trie::step_short_link(std::vector<int> &symbols_id, char symbol) {
    current_state = current_state->pw; // прошли по суффиксной ссылке
    if (current_state != root) {
        return false;
    } else {
        step_down(symbols_id, symbol);
        return true;
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

    void Step(std::vector<int> &symbols_id, int left_q);

    void Print(int left_q);

private:
    int counter = 1;
    int words_count; // сколько всего слов
    const int state_size; // длина входной строки веместе со всеми ???
    const std::deque<int> &shifts; // на сколько нужно сдвигать
    std::deque<int> state; // текущая очередь состояний
    std::deque<int> answer; // то, что возвращать в ответ
};

void Trie::defLink(std::deque<std::tuple<std::shared_ptr<Node>, char, std::shared_ptr<Node>>> &root_deque) {
    while (!root_deque.empty()) {
        auto element = root_deque.back();
        root_deque.pop_back();

        std::shared_ptr<Node> root_ = std::get<0>(element);
        char symbol = std::get<1>(element);
        std::shared_ptr<Node> current_ = std::get<2>(element);
        std::shared_ptr<Node> ref = root_->pw; // TODO .lock();

        do {
            if (ref->go.count(symbol)) { // Если существует путь по нужному ребру
                current_->pw = ref->go[symbol];
                ref = root;
            } else {
                ref = ref->pw; // TODO .lock();
                if (ref->go.count(symbol)) { // Если существует путь по нужному ребру
                    current_->pw = ref->go[symbol];
                    ref = root;
                }
            }

            for (int id : current_->pw->word_num) { // родительские терминальные варшины (имеют один общий суффикс)
                current_->word_num.push_back(id);
            }

        } while (ref != root); // Пока не дойдем до корня

        for (const auto &iter : current_->go) // добавить в очередь детей !!!
        {
            root_deque.push_front(std::make_tuple(current_, iter.first, iter.second)); // обход дочерних вершин
        }
    }
}

bool Trie::Add(const std::string &key, int id) {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) {
            current = current->go[symbol] = std::make_shared<Node>();
            //current->pw = current->pw.lock(); // Также добавляем ссылку на root
            current->pw = root;
            //std::cout << symbol << " " << counter << "\n";
        } else {
            current = next->second;
        }
    }
    current->word_num.push_back(id); // номер слова
    return true;
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

void Pattern::Step(std::vector<int> &symbols_id, const int left_q) {
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

    std::vector<int> out;

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

// TODO: Вернуть weak_ptr !!!!

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



