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

std::vector<int> getShiftArray(const std::string &input_str) {
    int i = 1;
    int j = 0;

    std::vector<int> outputShiftArray(input_str.length(), 0);

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
    return outputShiftArray;
};

bool findIndexes(const std::string &word, std::vector<int> &outputShiftArray,
              int &shift_index, const char &symbol, int &symbol_index, std::vector<int> &findIndex) {
    if (symbol == word[shift_index]) {
        symbol_index++;
        shift_index++;
        if (shift_index == word.length()) {
            findIndex.push_back(symbol_index - word.length());
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

std::vector<int> findPattern(const std::string &word, const std::string &text) {
    std::vector<char> inputStr;

    std::vector<int> outputShiftArray = getShiftArray(word);

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
        read_next = findIndexes(word, outputShiftArray, shift_index, text[i],
                             symbol_index, findIndex);
    }

    return findIndex;
}

int main() {
    std::iostream::sync_with_stdio(false);
    std::string word;
    std::string text;
    std::cin >> word;
    std::cin >> text;

    std::vector<int> findIndex = findPattern(word, text);

    for (int i : findIndex) {
        std::cout << i << " ";
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


aba
mababamrabab
*/

