/*
4_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1)
в отсортированном массиве. Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
*/


#include <random>
#include <iostream>
#include <utility>
#include <vector>

class OrderStat {
public:
    explicit OrderStat(int_fast64_t order, std::vector<int_fast64_t> arr);
    int_fast64_t call(); // запуск основного метода
private:
    int_fast64_t order;
    std::vector<int_fast64_t> array;
    int_fast64_t partition(int_fast64_t leftPointer, int_fast64_t rightPointer); // реализация алгоритма partition
    int_fast64_t getPointer(int_fast64_t leftPointer, int_fast64_t rightPointer); // вернуть медиану
    static int_fast64_t getRand(int_fast64_t leftPointer, int_fast64_t rightPointer); // случайный элемент
};

int_fast64_t OrderStat::getRand(int_fast64_t leftPointer, int_fast64_t rightPointer) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(leftPointer, rightPointer);
    return dis(gen); // случайный int из промежутка
}

int_fast64_t OrderStat::getPointer(int_fast64_t leftPointer, int_fast64_t rightPointer) {
    int_fast64_t s1, s2, s3;
    s1 = getRand(leftPointer, rightPointer);
    s2 = getRand(leftPointer, rightPointer);
    s3 = getRand(leftPointer, rightPointer);

    if ((array[s1] >= array[s2]) and (array[s2] >= array[s3])) {
        return s2;
    } else if ((array[s2] >= array[s1]) and (array[s1] >= array[s3])) {
        return s1;
    } else return s3;

}

OrderStat::OrderStat(int_fast64_t ord, std::vector<int_fast64_t> arr) {
    order = ord;
    array = std::move(arr);
}

int_fast64_t OrderStat::call() {
    int_fast64_t leftPointer = 0;
    int_fast64_t rightPointer = array.size();
    int_fast64_t pointer = partition(leftPointer, rightPointer); // нулевая итерация
    while (true) {
        if (pointer == order) {
            return array[pointer];
        } else if (pointer > order) {
            pointer = partition(leftPointer, pointer);
        } else {
            pointer = partition(pointer, rightPointer);
        }
    }
}

int_fast64_t OrderStat::partition(int_fast64_t leftPointer, int_fast64_t rightPointer) {
    int localPointer = getPointer(leftPointer, rightPointer);
    std::swap(array[localPointer], array[array.size() - 1]);
    int_fast64_t val = array[array.size() - 1];

    int_fast64_t i = leftPointer;
    int_fast64_t j = leftPointer;

    while (j < array.size() - 1) {
        if (array[j] > val) {
            j++;
        } else {
            std::swap(array[i], array[j]);
            i++;
            j++;
        }
    }
    std::swap(array[i], array[array.size() - 1]);
    return i;
}

int main() {
    int_fast64_t arraySize;
    int_fast64_t ord;
    int_fast64_t newElement;

    std::vector<int_fast64_t> arr;
    std::cin >> arraySize >> ord;

    for (int_fast64_t i = 0; i < arraySize; ++i) {
        std::cin >> newElement;
        arr.push_back(newElement);
    }

    OrderStat orderStat(ord, arr);
    std::cout << orderStat.call();

    return 0;
}
