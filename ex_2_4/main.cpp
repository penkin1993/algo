#include <iostream>
#include <vector>

/*
2_4. Скользящий максимум
Ограничение времени	2 секунды
Ограничение памяти	5Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Дан массив целых чисел A[0..n), n не превосходит 100 000. Так же задан размер некотрого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне. Скорость работы O(n log n), память O(n).

Формат ввода
Вначале вводится n - количество элементов массива. Затем вводится n строк со значением каждого элемента. Затем вводится k - размер окна.

Формат вывода
Разделенные пробелом значения максимумов для каждого положения окна.

Пример 1
Ввод	Вывод
1
1
1
1
Пример 2
Ввод	Вывод
3
1 2 3
3
3
Пример 3
Ввод	Вывод
3
1 2 3
1
1 2 3

 */


class Heap {
public:
    ~Heap();
    explicit Heap(int n);
    // Массив всех значений
    int * buffer = nullptr;
    // Массив окна
    std::vector<int> window = {};
    std::vector<int> memory = {};
    std::vector<int>res = {};

    int buffer_size = 0;
    int window_size = 0;

    void InitBuffer();

    void Iter(int iter_index);
    void BuildHeap();

private:
    // Индкс теукщей позиции окна, используется в Iter

    void SiftDown(int i);

};

Heap::Heap(int n) {
    buffer_size = n;
    buffer = new int[n];
}


Heap::~Heap() {
    delete [] buffer;
}

void Heap::SiftDown(int i){
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    int largest = i;
    if (left < window_size && window[left] > window[i]) {
        largest = left;
    }
    if (right < window_size && window[right] > window[largest]){
        largest = right;
    }
    if (largest != i){
        std::swap(window[i], window[largest]);
        std::swap(memory[i], memory[largest]);
        SiftDown(largest);
    }
}

void Heap::BuildHeap() {
    for (int i = window_size / 2 - 1; i >= 0; --i){
        SiftDown(i);
    }
}

void Heap::InitBuffer(){
    for (int i = 0; i < window_size; i++){
        window.insert(window.end(), buffer[i]);
        memory.insert(memory.end(), i);
    }
    BuildHeap();
}

void Heap::Iter(int iter_index){
    int temp = 0;

    //  добавили максимальный элемент
    res.insert(res.end(), window[temp]);

    for (int j = 0; j < window_size; j++){
        if (memory[j] == 0){

            memory[j] = window_size - 1; // Если memory[j] == 0, то выпадает это число
            temp = j; // сохраняем индекс выпадения
        }
        else{
            memory[j] -= 1;
        }
    }
    //Заменяем
    window.at(temp) = buffer[iter_index];
    SiftDown(temp); //  просеивание
}

int main() {
    int commands_count = 0;

    std::cin >> commands_count;
    Heap heap(commands_count);

    for (int i = 0; i < heap.buffer_size; i++) {
        std::cin >> heap.buffer[i];
    }
    std::cin >> heap.window_size;
    heap.InitBuffer();

    for (int i = heap.window_size; i < heap.buffer_size ; i++) {
        heap.Iter(i);
    }

    heap.BuildHeap();
    heap.res.insert(heap.res.end(), heap.window[0]);

    for (int i = 0; i < heap.buffer_size - heap.window_size + 1; i++) {
        std::cout << heap.res[i] << " ";
    }
}

