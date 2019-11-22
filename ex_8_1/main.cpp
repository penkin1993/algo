/*
Задача 8. Хеш-таблица (10 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
 удаления строки из множества и проверки принадлежности данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.


Формат входных данных
Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
Тип операции  – один из трех символов:
    +  означает добавление данной строки в множество;
    -  означает удаление  строки из множества;
    ?  означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
Формат выходных данных
Программа должна вывести для каждой операции одну из двух строк OK или FAIL.
Для операции '?': OK, если элемент присутствует во множестве. FAIL иначе.
Для операции '+': FAIL, если добавляемый элемент уже присутствует во множестве и потому не может быть добавлен. OK иначе.
Для операции '-': OK, если элемент присутствовал во множестве и успешно удален. FAIL иначе.

*/


#include <assert.h>
#include <iostream>
#include <string>
#include <cstring>

class HashTable {

public:
    explicit HashTable(size_t initial_size);

    ~HashTable();

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

    HashTable(const HashTable &) = delete;

    HashTable(HashTable &&) = delete;

    HashTable &operator=(const HashTable &) = delete;

    HashTable &operator=(HashTable &&) = delete;

private:
    struct HashTableNode {
        HashTableNode() = default;

        std::string key = "__NULL__";
        bool is_filled = false;
    };

    HashTableNode *table = nullptr;
    int capacity;
    int size = 0;

    static int Hash(const char *str, int m);

    static int HashProba(int hash_index, int i, int m);

    void Expand();
};

HashTable::HashTable(size_t initial_size) {
    table = new HashTableNode[initial_size];
    capacity = initial_size;
}


HashTable::~HashTable() {
    delete[] table;
}


int HashTable::Hash(const char *str, int m) { // хэш
    int hash = 0;
    for (; *str != 0; ++str)
        hash = (hash * 41 + *str) % m;
    return hash;
}

int HashTable::HashProba(int hash_index, int i, int m) { // функция пробирования
    return (hash_index + i + 1) % m;
}

bool HashTable::Has(const std::string &key) const {
    assert(!key.empty());

    char cstr[key.length() + 1];
    strcpy(cstr, key.c_str()); // приведение строки к массиву
    int hash_index = Hash(cstr, capacity); // первоначальный хэщ

    for (int i = 0; i < capacity; i++) {
        if (table[hash_index].key == key) {
            return true;
        } else if (!table[hash_index].is_filled) {
            return false;
        }
        hash_index = HashProba(hash_index, i, capacity);
    }
    return false;
}

bool HashTable::Remove(const std::string &key) {
    assert(!key.empty());

    char cstr[key.length() + 1];
    strcpy(cstr, key.c_str()); // приведение строки к массиву
    int hash_index = Hash(cstr, capacity); // первоначальный хэщ

    for (int i = 0; i < capacity; i++) {
        if (table[hash_index].key == key) {
            table[hash_index].key = "__NULL__";
            size++;
            return true;

        } else if (!table[hash_index].is_filled) {
            return false;
        }
        hash_index = HashProba(hash_index, i, capacity);
    }
    return false;
}

bool HashTable::Add(const std::string &key) {
    assert(!key.empty());
    char cstr[key.length() + 1];
    strcpy(cstr, key.c_str()); // приведение строки к массиву
    int hash_index = Hash(cstr, capacity); // первоначальный хэш

    int *add_index = nullptr;

    for (int i = 0; i < capacity; i++) {
        if (table[hash_index].key == key) {
            return false;
        }
        if ((table[hash_index].key == "__NULL__") && table[hash_index].is_filled) {
            add_index = &hash_index;

        } else if (!table[hash_index].is_filled) {
            if (add_index == nullptr) {
                add_index = &hash_index;
            }

            table[*add_index].key = key;
            table[*add_index].is_filled = true;
            size++;
            Expand();
            return true;
        }
        hash_index = HashProba(hash_index, i, capacity);
    }
    return false;
}

void HashTable::Expand() {
    if (size > (capacity * 3 / 4)) {
        size = 0;

        int old_capacity = capacity;
        capacity *= 2;

        HashTableNode *oldtable = nullptr;
        oldtable = table;

        table = nullptr;
        table = new HashTableNode[capacity];


        for (int i = 0; i < old_capacity; i++) {
            if (oldtable[i].key != "__NULL__") {
                Add(oldtable[i].key);
            }
        }
        delete[] oldtable;
    }
}


int main() {
    HashTable table(8);
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}

