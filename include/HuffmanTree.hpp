#pragma once

#include <cmath>
#include <fstream>
#include <iostream>

#include "CodeList.hpp"
#include "MinList.hpp"

using namespace std;

class TreeNode {
   public:
    TreeNode() : TreeNode(0){};

    TreeNode(int f) : TreeNode('\0', f){};

    TreeNode(char c, int f);

    char character;
    int freq;
    TreeNode *left, *right;
};

class AuxiliaryListNode {
   public:
    AuxiliaryListNode();

    AuxiliaryListNode(TreeNode *n);

    TreeNode *treeNode;
    AuxiliaryListNode *next;
};

class AuxiliaryList {
   public:
    AuxiliaryList();

    void addTreeNode(TreeNode *node);

    TreeNode *popMin();

    int getSize();

    int findMinFreq();

    void print();

   private:
    AuxiliaryListNode *first;
    int size;
};

class HuffmanTree {
   public:
    HuffmanTree();

    void buildTreeFromMinList(MinList list);

    void printTree();

    void buildCodeListFromTree();

    void compressFile(string inputFile, string outputFile);

    void decompressFile(string inputFile, string outputFile);

   private:
    void recursivePostOrderPrint(TreeNode *t, int level = 0);

    void recursiveBuildCodeListFromTree(int binaryDigits[], int index, TreeNode *node);

    void printCodeListIntoFile(ofstream &output);

    void createTreeFromFile(ifstream &input, int totalChars, int totalCodes);

    void decToBin(int *binArr, int dec, int size);

    int binToDec(int *binArr, int size);

    TreeNode *root;
    CodeList codeList;
};