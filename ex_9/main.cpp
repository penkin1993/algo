#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <map>
#include <functional>

typedef unsigned char byte;

class BitsWriter {
public:
    void WriteBit(bool bit);

    void WriteByte(unsigned char byte);

    std::vector<unsigned char> GetResult(bool add_len);

private:
    std::vector<unsigned char> buffer_;
    unsigned char accumulator_ = 0;
    int bits_count_ = 0;
};

void BitsWriter::WriteBit(bool bit) {
    // Ставим бит в аккумулятор на нужное место
    accumulator_ |= static_cast<unsigned char>(bit) << bits_count_;
    ++bits_count_;
    if (bits_count_ == 8) {
        bits_count_ = 0;
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
    }
}

void BitsWriter::WriteByte(unsigned char byte) {
    if (bits_count_ == 0) {
        buffer_.push_back(byte);
    } else {
        accumulator_ |= byte << bits_count_;
        buffer_.push_back(accumulator_);
        accumulator_ = byte >> (8 - bits_count_);
    }
}

std::vector<unsigned char> BitsWriter::GetResult(bool add_len) {
    if (bits_count_ != 0) {
        // Добавляем в буфер аккумулятор, если в нем что-то есть. Все правильно. Сколько ЗНАЧАЩИХ
        buffer_.push_back(accumulator_);
    }
    //std::cout << "bits_count_ " << bits_count_ << "\n";

    if (add_len) {
        buffer_.push_back(static_cast<unsigned char>(bits_count_));
        //bits_count_ = 0;
        //return buffer_; //std::move(buffer_);
    }
    accumulator_ = 0;
    bits_count_ = 0;

    if (add_len){
        return buffer_;
    }
    else{
        return std::move(buffer_);
    }
}


struct Node {
    int weight;
    byte symbol = '\0';
    Node *left = nullptr;
    Node *right = nullptr;

    Node(int weight_, byte symbol_, Node *left_, Node *right_)
            : weight(weight_), symbol(symbol_), left(left_), right(right_) {
    }
};

struct CompareWeight {
    bool operator()(Node *const &p1, Node *const &p2) {
        return p1->weight > p2->weight;
    }
};

std::priority_queue<Node *, std::vector<Node *>, CompareWeight> get_queue(std::deque<byte> &input) {
    std::map<byte, int> freq;

    // заполняем список
    for (auto c: input) {
        if (freq.find(c) == freq.end())
            freq[c] = +1;
        else
            ++freq[c];
    }

    std::priority_queue<Node *, std::vector<Node *>, CompareWeight> q;

    for (std::pair<byte, int> pair: freq) {
        q.push(new Node(pair.second, pair.first, nullptr, nullptr));
    }
    return q;
}

Node *get_tree(std::priority_queue<Node *, std::vector<Node *>, CompareWeight> &queue) {
    if (queue.empty()) {
        return nullptr;
    }
    while (queue.size() > 1) {
        Node *node_1 = queue.top();
        queue.pop();

        Node *node_2 = queue.top();
        queue.pop();

        Node *new_node = new Node(node_1->weight + node_2->weight, '\0', node_1, node_2);

        queue.push(new_node);
    }

    Node *new_node_ = queue.top();
    queue.pop();

    return new_node_;
}

void get_map(Node *root, std::map<byte,
        std::vector<int>> &map_symbols,
             std::vector<int> acc,
             std::deque<int> &tree_structure,
             std::deque<byte> &symbol_deque) {
    // 0 -- идти вниз. по дефолту влево. Если до подъема ходили налево, то направо
    // 1 -- идти наверх ???
    // определять по push_back ???

    if (root->symbol != '\0') {
        /////////////////////////////////////
        for (int i = 0; i < acc.size(); i++) {
            std::cout << acc[i] << " ";
        }
        std::cout << root->symbol << "\n";
        /////////////////////////////////////
        symbol_deque.push_back(root->symbol);

        map_symbols.insert({root->symbol, acc});
        tree_structure.push_back(1); // поднимаемся выше
    } else {

        acc.push_back(0);
        tree_structure.push_back(0); // опускаамся влево
        get_map(root->left, map_symbols, acc, tree_structure, symbol_deque);

        acc.pop_back();
        acc.push_back(1);

        tree_structure.push_back(0); // опускаемся вправо
        get_map(root->right, map_symbols, acc, tree_structure, symbol_deque);

        tree_structure.push_back(1); // поднимаемся выше
    }
}

std::vector<unsigned char> encode(const std::map<byte, std::vector<int>> &map_symbols, std::deque<byte> &original,
                                  std::deque<int> &tree_structure, std::deque<byte> &symbol_deque) {
    byte symbol;
    bool bit;
    std::vector<int> code;
    BitsWriter bits_writer;

    // сообщение, байт(сколько в последнем байте не фиктивно) | дерево, байт(сколько не фиктивно в дереве), байт(длина дерева) |
    // словарь(порядок дерева), байт(длина словаря)

    while (!original.empty()) { // сообщение
        symbol = original.front();
        original.pop_front();
        code = map_symbols.find(symbol)->second;
        //std::cout << symbol << " ";
        for (int i = 0; i < code.size(); i++) {
            //std::cout << code[i];
            bits_writer.WriteBit(code[i]);
        }
        //std::cout << "\n";
    }
    bits_writer.GetResult(true); //  байт(сколько в последнем байте не фиктивно) //

    int tree_len = tree_structure.size();
    while (!tree_structure.empty()) { // дерево
        bit = tree_structure.front();
        //std::cout <<" bit "<< bit << "\n";
        tree_structure.pop_front();
        bits_writer.WriteBit(bit);
    }

    bits_writer.GetResult(true); // байт(сколько не фиктивно в дереве)
    bits_writer.WriteByte(tree_len); // байт(длина дерева)

    int symbol_deque_len = symbol_deque.size();

    //std::cout << "symbol_deque_len " <<  symbol_deque_len << "\n";
    //std::cout << "symbol_deque " <<  symbol_deque[0] << "\n";
    //std::cout << "symbol_deque " <<  symbol_deque[1] << "\n";

    while (!symbol_deque.empty()) { // словарь(порядок дерева)
        symbol = symbol_deque.front();
        symbol_deque.pop_front();
        bits_writer.WriteByte(symbol);
    }

    //bits_writer.GetResult(false);
    bits_writer.WriteByte(symbol_deque_len);  // длина массива

    std::vector<unsigned char> result =
            std::move(bits_writer.GetResult(false));

    /*
    std::cout << "USSR ";
    byte a = 'a';
    for (int i = 0; i < 8; ++i) {
        std::cout << ((a >> i) & 1);
    }
    std::cout << " USSR ";


    for (unsigned char byte : result) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
*/
    //result = std::move(bits_writer.GetResult(false));
     /*
    std::vector<unsigned char> result =
            std::move(bits_writer.GetResult());

    for (unsigned char byte : result) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
*/

    return result;
}

void Encode(std::deque<byte> &original, std::vector<byte> &compressed) {

    std::priority_queue<Node *, std::vector<Node *>, CompareWeight> q = get_queue(
            original); // очередь для построения дерева Хаффмана

    Node *root_node = get_tree(q); // дерево Хаффмана
    std::map<byte, std::vector<int>> map_symbols; // Хэш таблица для кодирования. Символ и вектор из 0 и 1
    std::vector<int> acc;
    std::deque<int> tree_structure; // вектор для хранения структуры дерева
    std::deque<byte> symbol_deque;

    get_map(root_node, map_symbols, acc, tree_structure, symbol_deque);
    tree_structure.pop_back(); // Убрали лишний символ

    compressed = encode(map_symbols, original, tree_structure, symbol_deque);
    /*/////////////////////////////////////////////////
    std::cout << "\n|";
    for (int i = 0; i < tree_structure.size(); i++){
        std::cout << tree_structure[i];
    }
    std::cout << "|\n";
    ////////////////////////////////////////////////*/
}


void Decode(std::vector<byte> &compressed, std::deque<byte> &original) {
    // сообщение, байт(сколько в последнем байте фиктивно) | дерево, байт(сколько фиктивно в дереве), байт(длина дерева)
    // | словарь(порядок дерева), байт(длина словаря)

    int symbol_deque_len = compressed.back(); //  байт(длина словаря)
    compressed.pop_back();









    for (unsigned char byte : compressed) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
    std::cout << "\n";

    std::cout << "symbol_deque_len "<< symbol_deque_len;
    //std::cout << "symbol_deque_len "<< compressed.size();


    // TODO Обратная операция к кодированию ???


    // TODO: Tree reconstruct. В отдельный метод
}


int main() {
    std::deque<byte> input;
    std::deque<byte> output;
    std::map<byte, int> count;
    std::ifstream file;

    file.open("../test1.txt");
    byte value;

    ////////////////////////////////////////////////////////////////////////
    if (!file) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while (file >> value) {
        input.push_back(value);
    }
    file.close();
    ////////////////////////////////////////////////////////////////////////
    std::vector<byte> compressed;
    Encode(input, compressed);
    Decode(compressed, output);

    //for (int i = 0; i < input.size(); i++){ // считано все парвилтьно. TODO: Как считывать пробелы ???
    //    std::cout << input[i] << " ";
    //}

    /*
    std::priority_queue<Node*, std::vector<Node*>, CompareWeight> q = get_queue(input); // очередь для построения дерева Хаффмана

    Node* root_node = get_tree(q); // дерево Хаффмана
    std::map<byte, std::vector<int>> map_symbols; // Хэш таблица для кодирования. Символ и вектор из 0 и 1
    std::vector<int> acc;

    get_map(root_node, map_symbols, acc);
    */



    //std::cout << map_symbols.size() << " size \n";
    //for(auto elem : map_symbols)
    //{
    //    std::cout << elem.first  << "  "<< elem.second.size()  << "\n";
    //}

    return 0;
};











//std::cout << map_symbols.find(input[0])->first <<" "<< input[0] << " USSR";
/*

int main() {
    BitsWriter bits_writer;

    //bits_writer.WriteBit(true);
    //bits_writer.WriteBit(false);
    bits_writer.WriteByte(0);
    bits_writer.WriteByte('1');
    bits_writer.WriteByte('a');
    bits_writer.WriteByte('b');

    bits_writer.WriteBit(false);
    bits_writer.WriteBit(true);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(false);
    bits_writer.WriteBit(false);
    bits_writer.WriteByte(0xFE);
    bits_writer.WriteBit(true);
    bits_writer.WriteBit(false);

    std::vector<unsigned char> result =
            std::move(bits_writer.GetResult());

    for (unsigned char byte : result) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }
    return 0;
}
*/





/*
#include <cstring>
#include <iostream>
#include <vector>
//#include "Huffman.h"
​
std::vector<byte> ToVector(IInputStream& input)
{
    std::vector<byte> vec;
    byte value;
    while (input.Read(value))
    {
        vec.push_back(value);
    }
    return vec;
}
​
void FromVector(const std::vector<byte>& vec, IOutputStream& output)
{
    for (const byte& value: vec)
    {
        output.Write(value);
    }
}
​
void Encode(IInputStream& original, IOutputStream& compressed)
{
    std::vector<byte> temp = ToVector(original);
    FromVector(temp, compressed);
}
​
void Decode(IInputStream& compressed, IOutputStream& original)
{
    std::vector<byte> temp = ToVector(compressed);
    FromVector(temp, original);
}




//#include "Huffman.h"

struct IInputStream {
    // Возвращает false, если поток закончился
    bool virtual Read(byte &value) = 0;
};

struct IOutputStream {
    void virtual Write(byte value) = 0;
};


struct MyInputStream : public IInputStream {
    // Возвращает false, если поток закончился
    MyInputStream();

    bool Read(byte &value);
};

struct MyOutputStream : public IOutputStream {
    MyOutputStream();

    void Write(byte value);
};


MyInputStream::MyInputStream() {
    freopen(NULL, "rb", stdin);
}

bool MyInputStream::Read(byte &value) {
    int c = getc(stdin);
    bool res = c != EOF;
    value = c;
    return res;
}

MyOutputStream::MyOutputStream() {
    freopen(NULL, "wb", stdout);
}

void MyOutputStream::Write(byte value) {
    putc(value, stdout);
}


// Метод архивирует данные из потока original
void Encode(IInputStream &original, IOutputStream &compressed);


// Метод восстанавливает оригинальные данные
void Decode(IInputStream &compressed, IOutputStream &original);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "Huffman.h"

static void copyStream(IInputStream &input, IOutputStream &output) {
    byte value;
    while (input.Read(value)) {
        output.Write(value);
    }
}


void Encode(IInputStream &original, IOutputStream &compressed) {
    copyStream(original, compressed);
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    copyStream(compressed, original);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

