#include "HuffmanTree.hpp"

TreeNode::TreeNode(char c, int f) : character(c), freq(f) {
    left = nullptr;
    right = nullptr;
};

AuxiliaryListNode::AuxiliaryListNode() {
    treeNode = nullptr;
    next = nullptr;
};

AuxiliaryListNode::AuxiliaryListNode(TreeNode *n) {
    treeNode = n;
    next = nullptr;
};

AuxiliaryList::AuxiliaryList() {
    first = nullptr;
    size = 0;
};

void AuxiliaryList::addTreeNode(TreeNode *node) {
    if (!first) {
        first = new AuxiliaryListNode(node);
        size++;
        return;
    }

    AuxiliaryListNode *itr = first;
    while (itr) {
        if (itr->next) {
            itr = itr->next;
        } else {
            itr->next = new AuxiliaryListNode(node);
            size++;
            return;
        }
    }
};

TreeNode *AuxiliaryList::popMin() {
    if (!first) {
        throw runtime_error("Empty auxiliary list");
    }

    int min = findMinFreq();
    TreeNode *found;

    if (first->treeNode->freq == min) {
        found = first->treeNode;
        first = first->next;
        size--;
        return found;
    }

    AuxiliaryListNode *itr = first;
    while (itr) {
        if (itr->next->treeNode->freq == min) {
            found = itr->next->treeNode;
            itr->next = itr->next->next;
            size--;
            return found;
        }
        itr = itr->next;
    }
    throw runtime_error("Minimun frequency character not found");
}

int AuxiliaryList::getSize() {
    return size;
}

int AuxiliaryList::findMinFreq() {
    if (!first) {
        throw runtime_error("Empty auxiliary list");
    }

    AuxiliaryListNode *itr = first;
    int min = itr->treeNode->freq;
    while ((itr = itr->next)) {
        if (itr->treeNode->freq < min) min = itr->treeNode->freq;
    }

    return min;
}

void AuxiliaryList::print() {
    AuxiliaryListNode *itr = first;

    cout << "PRINTING AUXILIARY LIST" << endl;

    while (itr) {
        cout << "| \'" << itr->treeNode->character << "\' " << itr->treeNode->freq << endl;
        itr = itr->next;
    }
}

HuffmanTree::HuffmanTree() {
    root = nullptr;
}

void HuffmanTree::buildTreeFromMinList(MinList list) {
    TreeNode *l, *r, *top;

    AuxiliaryList auxList;
    Node *poppedAux;

    while (list.getSize() > 1 || auxList.getSize() + list.getSize() > 1) {
        if (!list.getSize() || (auxList.getSize() && auxList.findMinFreq() < list.findMinFreq())) {
            l = auxList.popMin();
        } else {
            poppedAux = list.popMin();
            l = new TreeNode(poppedAux->character, poppedAux->freq);
        }

        if (!list.getSize() || (auxList.getSize() && auxList.findMinFreq() < list.findMinFreq())) {
            r = auxList.popMin();
        } else {
            poppedAux = list.popMin();
            r = new TreeNode(poppedAux->character, poppedAux->freq);
        }

        top = new TreeNode(l->freq + r->freq);

        top->left = l;
        top->right = r;

        root = top;

        auxList.addTreeNode(top);
    }
}

void HuffmanTree::printTree() {
    cout << "PRINTING HUFFMAN TREE (postorder)" << endl;
    recursivePostOrderPrint(root);
}

void HuffmanTree::buildCodeListFromTree() {
    if (!root) throw runtime_error("Empty Huffman tree");

    codeList.clear();

    recursiveBuildCodeListFromTree(new int[32], 0, root);
}

void HuffmanTree::compressFile(string inputFile, string outputFile) {
    if (!root) throw runtime_error("Empty Huffman tree");
    if (!codeList.getSize()) throw runtime_error("Empty code list");

    ifstream input(inputFile);
    ofstream output(outputFile);

    int totalChars = root->freq;
    int codeListSize = codeList.getSize();

    output.write((char *)&totalChars, sizeof(int));    // imprime quantidade total de caracteres do arquivo original
    output.write((char *)&codeListSize, sizeof(int));  // imprime tamanho da lista de codes

    printCodeListIntoFile(output);

    unsigned char c, toWrite = 0;
    int currBinSize = 0;

    while (input >> noskipws >> c) {
        CodeListNode *codeNode = codeList.findByChar(c);

        for (int i = 0; i < codeNode->binarySize; i++) {
            if (currBinSize == 7) {
                if (codeNode->binaryDigits[i] == 1) {
                    toWrite++;
                    currBinSize = 0;
                } else {
                    currBinSize = 0;
                }
                output.write((char *)&toWrite, sizeof(char));
                toWrite = 0;
            } else if (currBinSize < 7) {
                if (codeNode->binaryDigits[i] == 1) {
                    toWrite++;
                    toWrite = toWrite << 1;
                    currBinSize++;
                } else if (codeNode->binaryDigits[i] == 0) {
                    toWrite = toWrite << 1;
                    currBinSize++;
                }
            }
        }
    }
    for (int i = 0; i < 7 - currBinSize; i++) {
        toWrite = toWrite << 1;
    }
    output.write((char *)&toWrite, sizeof(char));

    input.close();
    output.close();
}

void HuffmanTree::decompressFile(string inputFile, string outputFile) {
    unsigned char c;

    ifstream input(inputFile);
    ofstream output(outputFile);

    int totalChars, totalCodes;

    input.read((char *)&totalChars, sizeof(int));
    input.read((char *)&totalCodes, sizeof(int));

    int bin[8];
    int charCounter = 0;

    createTreeFromFile(input, totalChars, totalCodes);

    buildCodeListFromTree();

    TreeNode *treeItr = root;

    input.read((char *)&c, sizeof(char));

    decToBin(bin, (int)c, 8);

    for (int i = 0; i < 8 && charCounter < totalChars; i++) {
        if (treeItr->left || treeItr->right) {
            if (i != 7) {
                if (bin[i] == 0) {
                    treeItr = treeItr->left;
                }
                if (bin[i] == 1) {
                    treeItr = treeItr->right;
                }
            } else {
                if (bin[i] == 0) {
                    treeItr = treeItr->left;
                }
                if (bin[i] == 1) {
                    treeItr = treeItr->right;
                }
                if (input.read((char *)&c, sizeof(char))) {
                    decToBin(bin, (int)c, 8);
                    i = -1;
                } else {
                    break;
                }
            }
        } else {
            charCounter++;
            output.write(&treeItr->character, sizeof(char));
            treeItr = root;
            i--;
        }
    }

    input.close();
    output.close();
}

void HuffmanTree::recursivePostOrderPrint(TreeNode *t, int level) {
    if (t != nullptr) {
        for (int i = 0; i < level; i++) {
            cout << '\t';
        }
        cout << "> \'" << t->character << "\' " << t->freq;

        if (t->character) {
            CodeListNode *codeNode = codeList.findByChar(t->character);
            cout << " (";
            for (int i = 0; i < codeNode->binarySize; i++) {
                cout << codeNode->binaryDigits[i];
            }
            cout << ")";
        }

        cout << endl;
        recursivePostOrderPrint(t->left, level + 1);
        recursivePostOrderPrint(t->right, level + 1);
    }
}

void HuffmanTree::recursiveBuildCodeListFromTree(int binaryDigits[], int index, TreeNode *node) {
    if (node->left) {
        binaryDigits[index] = 0;
        recursiveBuildCodeListFromTree(binaryDigits, index + 1, node->left);
    }
    if (node->right) {
        binaryDigits[index] = 1;
        recursiveBuildCodeListFromTree(binaryDigits, index + 1, node->right);
    }

    if (!node->left && !node->right) {
        CodeListNode *codeNode = new CodeListNode(node->character, index);
        for (int i = 0; i < index; i++) {
            codeNode->binaryDigits[i] = binaryDigits[i];
        }

        codeList.add(codeNode);
    }
}

void HuffmanTree::printCodeListIntoFile(ofstream &output) {
    CodeListNode *itr = codeList.getFirst();

    while (itr) {
        int dec = binToDec(itr->binaryDigits, itr->binarySize);

        output.write(&itr->character, sizeof(char));
        output.write((char *)&itr->binarySize, sizeof(int));
        output.write((char *)&dec, sizeof(int));
        itr = itr->next;
    }
}

void HuffmanTree::createTreeFromFile(ifstream &input, int totalChars, int totalCodes) {
    unsigned char c;
    int binSize = 0, decValue = -1;

    TreeNode *newRoot = new TreeNode(totalChars);
    TreeNode *newTreeItr = newRoot;

    for (int i = 0; i < totalCodes; i++) {
        input.read((char *)&c, sizeof(char));
        input.read((char *)&binSize, sizeof(int));
        input.read((char *)&decValue, sizeof(int));

        int bin[binSize];
        decToBin(bin, decValue, binSize);

        for (int j = 0; j < binSize; j++) {
            if (bin[j] == 0) {
                if (!newTreeItr->left) {
                    newTreeItr->left = new TreeNode();
                }
                newTreeItr = newTreeItr->left;
            } else if (bin[j] == 1) {
                if (!newTreeItr->right) {
                    newTreeItr->right = new TreeNode();
                }
                newTreeItr = newTreeItr->right;
            }
        }
        newTreeItr->character = c;
        newTreeItr = newRoot;
    }

    root = newRoot;
}

void HuffmanTree::decToBin(int *binArr, int dec, int size) {
    for (int i = 1; i <= size; i++) {
        if (dec >= pow(2, size - i) && dec < pow(2, size - i + 1)) {
            binArr[i - 1] = 1;
            dec -= (int)pow(2, size - i);
        } else {
            binArr[i - 1] = 0;
        }
    }
}

int HuffmanTree::binToDec(int *binArr, int size) {
    int dec = 0;
    for (int i = 1; i <= size; i++) {
        dec += pow(2, size - i) * binArr[i - 1];
    }
    return dec;
}