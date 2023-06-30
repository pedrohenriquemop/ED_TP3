#include <math.h>

#include <fstream>
#include <iostream>

#include "HuffmanTree.hpp"
#include "MinList.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // setvbuf(stdout, nullptr, _IOFBF, 1000);

    if (argc != 4) {
        if (argc < 4)
            throw runtime_error("Not enough arguments");
        else
            throw runtime_error("Invalid arguments");
    } else if ((string)argv[3] != "-c" && (string)argv[3] != "-d") {
        throw runtime_error("Invalid arguments");
    }

    string file1 = (string)argv[1];
    string file2 = (string)argv[2];
    string op = (string)argv[3];

    ifstream input(file1);

    if (!input.is_open()) {
        string str = "Failed to open file \'";
        throw runtime_error(str.append(file1).append("\'"));
    }

    HuffmanTree tree;

    if (op == "-c") {
        string str = "Compactando arquivo \'";
        cout << str.append(file1).append("\'...") << endl;

        MinList list;
        char c;

        while (input >> noskipws >> c) {
            list.add(c);
        }

        tree.buildTreeFromMinList(list);
        tree.buildCodeListFromTree();

        tree.compressFile(file1, file2);

        str = "Arquivo compactado com sucesso para \'";

        cout << str.append(file2).append("\'") << endl;
    } else if (op == "-d") {
        string str = "Descompactando arquivo \'";
        cout << str.append(file1).append("\'...") << endl;

        tree.decompressFile(file1, file2);

        str = "Arquivo descompactado com sucesso para \'";

        cout << str.append(file2).append("\'") << endl;
    }

    input.close();

    return 0;
}