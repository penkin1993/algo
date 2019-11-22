#include <iostream>

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
    //int * buffer = nullptr;
    // Массив окна
    int_fast8_t * window = nullptr;
    int_fast8_t * memory = nullptr;

    int buffer_size = 0;
    int window_size = 0;

    int Iter(int iter_index, int val);
    void BuildHeap();

private:
    // Индкс теукщей позиции окна, используется в Iter

    void SiftDown(int i);

};

Heap::Heap(int n) {
    buffer_size = n;
    //buffer = new int[n];
}

Heap::~Heap() {
    delete [] window;
    delete [] memory;
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


int Heap::Iter(int iter_index, int val){
    int temp = 0;
    //  добавили максимальный элемент
    //res[iter_index - window_size] = window[0];

    for (int j = 0; j < window_size; j++){
        if (memory[j] != 0){
            memory[j] -= 1;
        }
        else{
            memory[j] = window_size - 1; // Если memory[j] == 0, то выпадает это число
            temp = j; // сохраняем индекс выпадения
        }
    }
    window[temp] = val;

    std::swap(window[temp], window[0]); // поднимаем элемент наверх кучи
    std::swap(memory[temp], memory[0]);

    SiftDown(0); //  просеивание

    return window[0];
}

int main() {
    int commands_count = 0;

    std::cin >> commands_count;
    Heap heap(commands_count);

    int buffer[commands_count];

    for (int i = 0; i < heap.buffer_size; i++) {
        std::cin >> buffer[i];
    }
    std::cin >> heap.window_size;

    heap.window = new int_fast8_t[heap.window_size];
    heap.memory = new int_fast8_t[heap.window_size];
    int res[heap.buffer_size + heap.window_size - 1];

    for (int i = 0; i < heap.window_size; i++){
        heap.window[i] = buffer[i];
        heap.memory[i] = i;
    }
    heap.BuildHeap();

    res[0] = heap.window[0];

    for (int i = heap.window_size; i < heap.buffer_size + 1 ; i++) {
        res[i - heap.window_size + 1] = heap.Iter(i, buffer[i]);
    }
    //res[heap.buffer_size - heap.window_size] = heap.window[0];

    for (int i = 0; i < heap.buffer_size - heap.window_size + 1; i++) {
        std::cout << res[i] << " ";
    }
}

/*
 8
 4 4 4 0 0 0 1 -12
 3
*/


