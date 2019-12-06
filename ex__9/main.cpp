#include "Htree.h"

int main() {

    //bool encode = true;
    bool encode = false;

    std::string input_file, output_file, decoded_file, compressed_file;
    input_file = "file.txt";
    output_file = "archive.bin";
    decoded_file = "decoded.txt";

    if (encode) {
        IInputStream input(input_file.c_str());
        IOutputStream output(output_file.c_str());

        Encode(input, output);
    } else {
        IInputStream compressed(output_file.c_str());
        IOutputStream decoded(decoded_file.c_str());

        Decode(compressed, decoded);
    }

    return 0;
}
