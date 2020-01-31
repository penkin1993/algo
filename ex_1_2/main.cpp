#include <iostream>

/*
1_2. Дек с динамическим буфером
Ограничение времени	1 секунда
Ограничение памяти	5Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Реализовать дек с динамическим зацикленным буфером.

Обрабатывать команды push * и pop *.

Формат ввода
В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Если дана команда pop *, то число b - ожидаемое значение. Если команда pop * вызвана для пустой структуры данных, то ожидается “-1”.
 */


class Deque {
public:
    ~Deque();
    // Итерация алгоритма
    bool step(int command, int value);
    // Добавление элемента сначала
    void push_front(int value);
    // Извлечение элемента сначала
    int pop_front();
    // Добавление элемента в конец
    void push_back(int value);
    // Извлечение элемента с конца
    int pop_back();

private:
    // Проверка на пустоту
    bool empty() const;
    // Указатель на динамический массив
    int * buffer = nullptr;
    // Индекс головы массива
    int head = 0;
    // Индекс хвоста массива
    int tail = 0;
    // Вместимость массива
    int capacity = 0;
    // Число непустых элементов массива
    int size = 0;
    // Метод отвечающий за увеличение массива
    void expand();
    // Метод отвечающий за уменьшение массива
    void squeeze();
    // Метод копирования массивов для
    void copy(int old_capacity, int new_capacity);
};


Deque::~Deque() {
    delete [] buffer;
}

bool Deque::empty() const {
    return size == 0;
}

void Deque::expand() {
    size +=1;
    if (size == 1){
        copy(0, 1);
        head = 0;
        tail = 0;
    }
    else if (size > capacity){
        copy(capacity, 2*capacity);
        head = 0;
        tail = capacity / 2 - 1;
    }
}

void Deque::squeeze() {
    size -=1;

    if (size == 0){
        copy(1, 0);
        head = 0;
        tail = 0;
        capacity = 0;
    }

    else if (2*size == capacity){
        copy(capacity, capacity / 2);
        head = 0;
        tail = capacity - 1;
    }
}

void Deque::copy(int old_capacity, int new_capacity) {
    capacity = new_capacity;

    if (new_capacity == 0){
        delete [] buffer;
        buffer = nullptr;
        return;
    }

    int * new_buffer = new int[new_capacity];

    for (int i = 0; i < std::min(old_capacity, new_capacity); i++){
        new_buffer[i] = buffer[(head + i) % old_capacity];
    }

    delete [] buffer;
    buffer = new_buffer;
}

void Deque::push_front(int value) {
    expand();
    head -=1;
    if (head < 0){
        head = capacity -1;
    }
    buffer[head] = value;
}

void Deque::push_back(int value) {
    expand();
    tail += 1;
    if (tail > capacity -1){
        tail = 0;
    }
    buffer[tail] = value;
}


int Deque::pop_front() {
    int value = buffer[head];
    head += 1;
    squeeze();
    if (head > capacity - 1){
        head = 0;
    }
    return value;
}

int Deque::pop_back() {
    int value = buffer[tail];
    tail -=1;
    squeeze();
    if (tail < 0){
        tail = capacity -1;
    }
    return value;
}

bool Deque::step(int command, int value){
    switch (command){
        case 1:
            push_front(value);
            return true;
        case 2:
            if (empty()) {
                if (value != -1) {
                    std::cout << "NO";
                    return false;
                }
            } else if (pop_front() != value){
                std::cout << "NO";
                return false;
            }
            return true;

        case 3:
            push_back(value);
            return true;
        case 4:
            if (empty()) {
                if (value != -1) {
                    std::cout << "NO";
                    return false;
                }
            } else if (pop_back() != value){
                std::cout << "NO";
                return false;
            }
            return true;
        default:
            std::cout << "NO";
            return false;
    }
}

int main() {
    int commands_count = 0;
    std::cin >> commands_count;
    Deque deque;
    bool is_cont;

    for (int i = 0; i < commands_count; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        is_cont = deque.step(command, value);
        if (!is_cont){
            return 0;
        }
    }
    std::cout << "YES";
    return 0;
}
