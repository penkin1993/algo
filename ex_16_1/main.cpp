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
*/

#include <iostream>
#include <vector>

void getShiftArray(std::vector<char> &input_str, std::vector<int> &outputShiftArray) {
    int i = 1;
    int j = 0;
    //input_str.pop_back();
    //outputShiftArray.pop_back();

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
              int &shift_index, char &symbol, int &symbol_index, std::vector<int> &findIndex) {
    if (symbol == inputStr[shift_index]) {
        symbol_index++;
        shift_index++;
        if (shift_index == inputStr.size()) {
            findIndex.push_back(symbol_index - inputStr.size());
        }
        return true;
    } else {
        if (shift_index == 0) {
            symbol_index++;
            return true;
        } else {
            shift_index = outputShiftArray[shift_index - 1];
            return false;
        }
    }
}

void findPattern(std::string &word, std::string &text) {
    std::vector<char> inputStr;
    std::vector<int> outputShiftArray;

    for (char symbol_ : word) { //Считывание в массив подстроки
        inputStr.push_back(symbol_);
        outputShiftArray.push_back(0);
    }

    getShiftArray(inputStr, outputShiftArray);

    int i = 0;
    int symbol_index = 0;
    int shift_index = 0;
    std::vector<int> findIndex;
    bool read_next = false;

    while (i < text.length()) //Считывание в массив подстроки
    {
        if (read_next) {
            i++;
        }
        read_next = findRows(inputStr, outputShiftArray, shift_index, text[i],
                             symbol_index, findIndex);
    }

    for (int i : findIndex) {
        std::cout << i << " ";
    }


}

int main() {
    std::iostream::sync_with_stdio(false);
    std::string word;
    std::string text;
    std::cin >> word;
    std::cin >> text;

    findPattern(word, text);
    return 0;
}

/*
abcabd
abcabeabcabcabd
9

aba
abacababa
0 4 6


aba
mababamrabab
*/






