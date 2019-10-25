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
    int buffer[10] = {1, 2, 8, -1, 2, 23, 4, 3, 3, 3};
    int buffer_len = 10;

    int old_element = NULL;
    int new_element = NULL;

    void InitBuffer();

    void SiftDown(int i);

    void BuildHeap();

private:

};

Heap::~Heap() {
}

void Heap::SiftDown(int i){
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    int largest = i;
    if (left < buffer_len && buffer[left] > buffer[i]) {
        largest = left;
    }
    if (right < buffer_len && buffer[right] > buffer[largest]){
        largest = right;
    }
    if (largest != i){
        std::swap(buffer[i], buffer[largest]);
        SiftDown(largest);
    }
}

void Heap::BuildHeap() {
    for (int i = buffer_len / 2 - 1; i >= 0; --i){
        SiftDown(i);
    }
}


/*
bool Queue::empty() const {
    return capacity == 0;
}

void Queue::expand() {
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

void Queue::squeeze() {
    size -=1;

    if (size == 0){
        copy(1, 0);
        head = 0;
        tail = 0;
    }

    else if (2*size == capacity){
        copy(capacity, capacity / 2);
        head = 0;
        tail = capacity - 1;
    }
}

void Queue::copy(int old_capacity, int new_capacity) {
    capacity = new_capacity;

    if (new_capacity == 0){
        delete [] buffer;
        buffer = nullptr;
        return;
    }

    int * new_buffer = new int[new_capacity];

    for (int i = 0; i < old_capacity; i++){
        new_buffer[i] = buffer[(head + i) % old_capacity];
    }

    delete [] buffer;
    buffer = new_buffer;
}

void Queue::push_front(int value) {
    expand();
    head -=1;
    if (head < 0){
        head = capacity -1;
    }
    buffer[head] = value;
}

void Queue::push_back(int value) {
    expand();
    tail += 1;
    if (tail > capacity){
        tail = 0;
    }
    if (capacity == 1){
        tail = 0;
    }
    buffer[tail] = value;
}


int Queue::pop_front() {
    int value = buffer[head];
    head +=1;
    squeeze();
    if (head > capacity - 1){
        head = 0;
    }
    return value;
}

int Queue::pop_back() {
    int value = buffer[tail];
    tail -=1;
    squeeze();
    if (tail < 0){
        tail = capacity -1;
    }
    return value;
}

*/
int main() {
    int commands_count = 0;
    std::cin >> commands_count;
    Heap heap;

    heap.BuildHeap();

    for (int i = 0; i < heap.buffer_len; i++){
        std::cout << heap.buffer[i] << " ";
    }

    // TODO: Как читать в массив !!!!

}

/*
 *
tijoforyou
Aug '13
If you are referring to the sample input like this:

7
12 34 56 78 76 54 32
then, you can use

cin >> sizeofarray;
for (int i = 0; i < sizeofarray; ++i)
{
    cin >> a*;
}
That is, you need not do anything special. Just read the array elements one-by-one like you would do otherwise.
 *
 */