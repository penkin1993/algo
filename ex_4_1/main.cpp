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

class OrderStat {
public:
    explicit OrderStat(int order, int * arr, int size);
    ~OrderStat();
    int call(); // запуск основного метода
private:
    int size;
    int order;
    int * array;
    int partition(int leftPointer, int rightPointer); // реализация алгоритма partition
    int getPointer(int leftPointer, int rightPointer); // вернуть медиану
    static int getRand(int leftPointer, int rightPointer); // случайный элемент
};

OrderStat:: ~OrderStat() {
    delete [] array;
}

int OrderStat::getRand(int leftPointer, int rightPointer) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(leftPointer, rightPointer);
    return dis(gen); // случайный int из промежутка
}

int OrderStat::getPointer(int leftPointer, int rightPointer) {
    int s1, s2, s3;
    s1 = getRand(leftPointer, rightPointer);
    s2 = getRand(leftPointer, rightPointer);
    s3 = getRand(leftPointer, rightPointer);

    if ((array[s1] >= array[s2]) and (array[s2] >= array[s3])) {
        return s2;
    } else if ((array[s2] >= array[s1]) and (array[s1] >= array[s3])) {
        return s1;
    } else return s3;

}

OrderStat::OrderStat(int ord, int * arr, int s) {
    order = ord;
    array = arr;
    size = s;
}

int OrderStat::call() {
    int leftPointer = 0;
    int rightPointer = size;
    while (true) {
        int pointer = partition(leftPointer, rightPointer);
        if (pointer == order) {
            return array[pointer];
        } else if (pointer > order) {
            rightPointer = pointer;
        } else {
            leftPointer = pointer;
        }
    }
}

int OrderStat::partition(int leftPointer, int rightPointer) {
    int localPointer = getPointer(leftPointer, rightPointer);
    std::swap(array[localPointer], array[rightPointer - 1]);
    int val = array[rightPointer - 1];

    int i = leftPointer;
    int j = leftPointer;

    while (j < rightPointer - 1) {
        if (array[j] > val) {
            j++;
        } else {
            std::swap(array[i], array[j]);
            i++;
            j++;
        }
    }
    std::swap(array[i], array[rightPointer - 1]);
    return i;
}

int main() {
    int arraySize = 0;
    int ord;
    int newElement;

    std::cin >> arraySize >> ord;
    int * arr = new int[arraySize];

    for (int i = 0; i < arraySize; ++i) {
        std::cin >> newElement;
        arr[i] = newElement;
    }
    OrderStat orderStat(ord, arr, arraySize);
    std::cout << orderStat.call();
}

