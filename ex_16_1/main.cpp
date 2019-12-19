/*
Задача 16. Поиск подстроки (5 баллов)
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта).
Вариант 2. С помощью z-функции.
Формат входного файла
Шаблон, символ перевода строки, строка.
Формат выходного файла
Позиции вхождения шаблона в строке.
Время: 100мс, память 3Mb.ц
stdin
stdout
aba
abacababa
0 4 6
*/

#include <iostream>
#include <vector>

void getShiftArray(std::vector<char> &input_str, std::vector<int> &outputShiftArray) {
    int i = 1;
    int j = 0;
    input_str.pop_back();
    outputShiftArray.pop_back();

    while (i != input_str.size()) {
        if (input_str[i] == input_str[j]) {
            outputShiftArray[i] = j + 1;
            i++;
            j++;
        } else if (j == 0) {
            outputShiftArray[i] = 0;
            i++;
        } else {
            j = outputShiftArray[j - 1];
        }
    }
};

bool findRows(std::vector<char> &inputStr, std::vector<int> &outputShiftArray,
              int &shift_index, char & symbol, int &symbol_index, std::vector<int> &findIndex) {
    if (symbol == inputStr[shift_index]) {
        symbol_index++;
        shift_index++;
        if (shift_index == inputStr.size()) {
            findIndex.push_back(symbol_index - inputStr.size());
        }
        return true;
    }
    else{
        if (shift_index == 0){
            symbol_index++;
            return true;
        }
        else{
            shift_index = outputShiftArray[shift_index - 1];
            return false;
        }
    }
}




int main() {
    char symbol = ' ';
    std::vector<char> inputStr;
    std::vector<int> outputShiftArray;

    while (symbol != '\n') //Считывание в массив подстроки
    {
        std::cin.get(symbol);
        inputStr.push_back(symbol);
        outputShiftArray.push_back(0);
    }

    //std::cout << inputStr.size() << "\n";
    //std::cout << outputShiftArray.size() << "\n";

    getShiftArray(inputStr, outputShiftArray);

    //for (int i = 0; i < inputStr.size(); i++){
    //    std::cout << inputStr[i];
    //}
    //std::cout << "\n";

    //for (int i = 0; i < outputShiftArray.size(); i++){
    //std::cout << inputStr[i];
    //    std::cout << outputShiftArray[i];
    //}
    int symbol_index = 0;
    int shift_index = 0;
    std::vector<int> findIndex;
    bool read_next = true;
    symbol = ' ';

    while (symbol != '\n') //Считывание в массив подстроки
    {
        //std::cout << shift_index;
        if (read_next) {
            std::cin.get(symbol);
            std::cout << symbol;
        }
        read_next = findRows(inputStr, outputShiftArray, shift_index, symbol,
                symbol_index, findIndex);
    } // TODO: Можно сразу писать !!!


    for (int i = 0; i < findIndex.size(); i++){
        std::cout << findIndex[i] << " ";
    }


    return 0;
}

/*
abcabd
abcabeabcabcabd
9

aba
abacababa

0 4 6
*/









