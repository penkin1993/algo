/*
3_3. Количество инверсий.

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод стандартный ввод или input.txt
Вывод стандартный вывод или output.txt
Дана последовательность целых чисел из диапазона (-1000000000 .. 1000000000).
Длина последовательности не больше 1000000. Числа записаны по одному в строке.
Количество чисел не указано. Пусть количество элементов n, и числа записаны в массиве a = a[i]: i из [0..n-1].
Требуется напечатать количество таких пар индексов (i,j) из [0..n-1], что (i < j и a[i] > a[j]).
Указание: количество инверсий может быть больше 4*1000000000 - используйте int64_t.
*/

#include <random>
#include <iostream>
#include <utility>
#include <vector>


class OrderStat{
public:
    explicit OrderStat(int_fast64_t order, std::vector<int_fast64_t> arr);
    int_fast64_t pointer;
    int_fast64_t order;
    std::vector<int_fast64_t> array;

    int_fast64_t call(); // запуск метода
    void partition(int_fast64_t leftPointer, int_fast64_t rightPointer); // реализация алгоритма partition
    static int_fast64_t getPointer(int_fast64_t leftPointer, int_fast64_t rightPointer); // вернуть опорный элемент элемент

private:

};

OrderStat::OrderStat(int_fast64_t ord, std::vector<int_fast64_t> arr){
    order = ord;
    array = std::move(arr);
}

int_fast64_t OrderStat::call() {


    return array[pointer];
}

int_fast64_t OrderStat::getPointer(int_fast64_t leftPointer, int_fast64_t rightPointer){
    std::uniform_real_distribution<int> distribution(leftPointer,rightPointer);
    return 0; // выбор медианы
}

void OrderStat::partition(int_fast64_t leftPointer, int_fast64_t rightPointer) {
    int localPointer = getPointer(leftPointer, rightPointer);

    // swap с последним
    //counter_run(0, array.size());
}



/*
void Inversion::counter_run(int64_t left_pointer, int64_t right_pointer){
    if (right_pointer - left_pointer <= 1){
        return;
    }
    else{
        int64_t mean_pointer = (left_pointer + right_pointer) / 2;

        counter_run(left_pointer, mean_pointer);
        counter_run(mean_pointer, right_pointer);
        counter_inv(left_pointer, mean_pointer, right_pointer);
    }
}

void Inversion::counter_inv(int64_t left_pointer, int64_t mean_pointer, int64_t right_pointer) {
    int i = left_pointer;
    int j = mean_pointer;
    std::vector<int64_t> new_part;

    while ((i < mean_pointer) && (j < right_pointer)){
        if (array[i] <= array[j]){
            new_part.push_back(array[i]);
            i += 1;
        }
        else{
            new_part.push_back(array[j]);
            counter += mean_pointer - i;
            j += 1;
        }
    }
    for (; i < mean_pointer; i++){
        new_part.push_back(array[i]);
    }
    for (; j < right_pointer; j++){
        new_part.push_back(array[j]);
    }

    for (i = 0; i < new_part.size(); i ++) {
        array[left_pointer + i] = new_part[i];
    }
}
*/

int main() {
    /*
    int_fast64_t arraySize;
    int_fast64_t ord;
    int_fast64_t newElement;

    std::vector<int_fast64_t> arr;
    std::cin >> arraySize >> ord;

    for (int_fast64_t i = 0; i < arraySize; ++i){
        std::cin >> newElement;
        arr.push_back(newElement);
    }

    OrderStat orderStat(ord, arr);
    */

    std::uniform_real_distribution<int> distribution(5,15);
    // TODO Проверить !!!


    //std::cout << orderStat.call() << "\n";

    //for (int i = 0; i < arraySize; ++i){
    //    std::cout << arr[i] << "\n";
    //}

    return 0;
}