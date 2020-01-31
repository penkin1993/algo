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

#include <iostream>
#include <vector>

class Inversion{
public:
    explicit Inversion(std::vector<int64_t> arr);
    int64_t call();

private:
    std::vector<int64_t> array;
    int64_t counter = 0; // счетчик числа инверсий
    void counter_run(int64_t left_pointer, int64_t right_pointer);
    void counter_inv(int64_t left_pointer, int64_t mean_pointer, int64_t right_pointer);
};

Inversion::Inversion(std::vector<int64_t> arr){
    array = std::move(arr);
}

int64_t Inversion::call(){
    counter_run(0, array.size());
    return counter;
}

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

int main() {
    std::vector<int64_t> arr;
    std::string line;
    getline(std::cin, line);
    while (!line.empty()) {
        arr.push_back(std::stoi(line));
        getline(std::cin, line);
    }
    Inversion inversion(arr);
    std::cout << inversion.call() << "\n";
    return 0;
}