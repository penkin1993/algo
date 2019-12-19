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

void getShiftArray(std::vector<char> & input_str, std::vector<int> & outputShiftArray){
    int i = 1;
    int j = 0;
    input_str.pop_back();
    outputShiftArray.pop_back();

    while (i != input_str.size()){
        if (input_str[i] == input_str[j]){
            outputShiftArray[i] = j + 1;
            i++;
            j++;
        }
        else if (j == 0){
            outputShiftArray[i] = 0;
            i++;
        } else{
            j = outputShiftArray[j - 1];
        }
    }
};

void findRows(std::vector<int> & outputShiftArray, char symbol, std::vector<int> & rowIndex){

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

    for (int i = 0; i < outputShiftArray.size(); i++){
        //std::cout << inputStr[i];
        std::cout << outputShiftArray[i];
    }

    std::vector<int> rowIndex;
    while (symbol != '\n') //Считывание в массив подстроки
    {
        std::cin.get(symbol); // TODO: Check the last symbol !!!!!!!
        findRows(outputShiftArray, symbol, rowIndex);
    }

    return 0;

    // TODO: 3. Шаг 2. Строку считывать на лету !!!
}
/*
abbaabbab

aba
abacababa
*/