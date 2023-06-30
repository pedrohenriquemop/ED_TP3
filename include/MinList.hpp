#pragma once

#include <fstream>
#include <iostream>

using namespace std;

class Node {
   public:
    Node(char c, int f) : character(c), freq(f){};

    char character;
    int freq;
    Node* next;
};

class MinList {
   public:
    MinList();

    void add(char c);

    Node* popMin();

    int getSize();

    int findMinFreq();

    void print();

   private:
    int size;
    Node* first;
};