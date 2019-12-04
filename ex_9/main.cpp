#include <iostream>
#include <vector>
#include <fstream>
#include <zconf.h>
#include <queue>
#include <map>
#include <functional>

typedef unsigned char byte;

class BitsWriter {
public:
    void WriteBit(bool bit);
    void WriteByte(unsigned char byte);

    std::vector<unsigned char> GetResult();

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

std::vector<unsigned char> BitsWriter::GetResult() {
    if (bits_count_ != 0) {
        // Добавляем в буфер аккумулятор, если в нем что-то есть.
        buffer_.push_back(accumulator_);
    }
    buffer_.push_back(static_cast<unsigned char>(bits_count_));
    return std::move(buffer_);
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

std::priority_queue<Node*, std::vector<Node*>, CompareWeight> get_queue(std::deque<byte> & input) {
    std::map<byte, int> freq;

    // заполняем список
    for (auto c: input) {
        if (freq.find(c) == freq.end())
            freq[c] = +1;
        else
            ++freq[c];
    }

    std::priority_queue<Node*, std::vector<Node*>, CompareWeight> q;

    for (std::pair<byte, int> pair: freq) {
        q.push(new Node(pair.second, pair.first, nullptr, nullptr));
    }
    return q;
}

Node* get_tree(std::priority_queue<Node*, std::vector<Node*>, CompareWeight> & queue) {
    if (queue.empty()) {
        return nullptr;
    }
    while (queue.size() > 1) {
        Node* node_1 = queue.top();
        queue.pop();

        Node* node_2 = queue.top();
        queue.pop();

        Node* new_node = new Node(node_1->weight + node_2->weight, '\0', node_1, node_2);

        queue.push(new_node);
    }

    Node* new_node_ = queue.top();
    queue.pop();

    return new_node_;
}

void get_map(Node* root, std::map<byte,
        std::vector<int>> & map_symbols,
        std::vector<int> acc,
        std::deque<int> & tree_structure,
        std::deque<byte> & symbol_deque) {
        // 0 -- идти вниз. по дефолту влево. Если до подъема ходили налево, то направо
        // 1 -- идти наверх ???
        // определять по push_back ???

    if (root->symbol != '\0'){
        /////////////////////////////////////
        for (int i = 0; i < acc.size(); i++){
            std::cout << acc[i] << " ";
        }
        std::cout << root-> symbol << "\n";
        /////////////////////////////////////
        symbol_deque.push_back(root -> symbol);
        // TODO: добавлять symbol в очередь !!!

        map_symbols.insert({root->symbol, acc});
        tree_structure.push_back(1); // поднимаемся выше
    }
    else{

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

// TODO: Функция кодирования сообщения !!!
// TODO: добавлять в хвост сами символы и соответствующее их число !!!
void Encode(std::deque<byte> &original, std::vector<byte> &compressed) {
    byte symbol;
    std::vector<int> code;

    std::priority_queue<Node*, std::vector<Node*>, CompareWeight> q = get_queue(original); // очередь для построения дерева Хаффмана

    Node* root_node = get_tree(q); // дерево Хаффмана
    std::map<byte, std::vector<int>> map_symbols; // Хэш таблица для кодирования. Символ и вектор из 0 и 1
    std::vector<int> acc;
    std::deque<int> tree_structure; // вектор для хранения структуры дерева
    std::deque<byte> symbol_deque;

    get_map(root_node, map_symbols, acc, tree_structure, symbol_deque); // TODO: Лишняя единица !!!

    BitsWriter bits_writer;

    while (!original.empty()){
        symbol = original.front();
        original.pop_front();
        code = map_symbols.find(symbol)->second;

        std::cout << symbol << " ";
        for (int i = 0; i < code.size(); i++){
            bits_writer.WriteBit(code[i]);
        }
    }

    //////////////////////////////////////////////////
    std::cout << "\n|";
    for (int i = 0; i < tree_structure.size(); i++){
        std::cout << tree_structure[i];
    }
    std::cout << "|\n";
    //////////////////////////////////////////////////

    // TODO: придерживаься интерфейса
    // TODO: добавить пары (символ, значение) и n












    std::vector<unsigned char> result =
            std::move(bits_writer.GetResult());

    for (unsigned char byte : result) {
        for (int i = 0; i < 8; ++i) {
            std::cout << ((byte >> i) & 1);
        }
        std::cout << " ";
    }




}


int main() {
    std::deque<byte> input;
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


*/



