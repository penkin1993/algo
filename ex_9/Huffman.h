#include <fstream>
#include <cstdio>

typedef unsigned char byte;

struct IInputStream {
    // Возвращает false, если поток закончился
    explicit IInputStream(const char *filename);

    ~IInputStream();

    bool Read(byte &value);

    FILE *file;
};

IInputStream::IInputStream(const char *filename) {
    file = fopen(filename, "rb");
}

IInputStream::~IInputStream() {
    fclose(file);
}

bool IInputStream::Read(byte &value) {
    int c = fgetc(file);
    bool res = c != EOF;
    value = c;
    return res;
}

struct IOutputStream {
    explicit IOutputStream(const char *filename);

    ~IOutputStream();

    void Write(byte value);

    FILE *file;
};

IOutputStream::IOutputStream(const char *filename) {
    file = fopen(filename, "wb");
}

IOutputStream::~IOutputStream() {
    fclose(file);
}

void IOutputStream::Write(byte value) {
    fputc(value, file);
}
