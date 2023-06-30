#pragma once

#include <iostream>

using namespace std;

class CodeListNode {
   public:
    CodeListNode();

    CodeListNode(char c, int bs) : character(c), binarySize(bs) {}

    char character;
    int binarySize;
    int binaryDigits[32];
    CodeListNode* next;
};

class CodeList {
   public:
    CodeList();

    void add(CodeListNode* node);

    CodeListNode* findByChar(char c);

    int getSize();

    CodeListNode* getFirst();

    void clear();

    void printCodeList();

   private:
    CodeListNode* first;
    int size;
};