#include "CodeList.hpp"

CodeListNode::CodeListNode() {
    character = '\0';
    binarySize = 0;
    next = nullptr;
}

CodeList::CodeList() {
    first = nullptr;
    size = 0;
}

void CodeList::add(CodeListNode* node) {
    if (!first) {
        first = node;
        size++;
        return;
    }

    CodeListNode* itr = first;
    while (itr) {
        if (itr->next) {
            itr = itr->next;
        } else {
            itr->next = node;
            size++;
            return;
        }
    }
};

CodeListNode* CodeList::findByChar(char c) {
    if (!first) {
        throw runtime_error("Empty code list");
    }

    CodeListNode* itr = first;
    while (itr) {
        if (itr->character == c) return itr;
        itr = itr->next;
    }
    return nullptr;
}

int CodeList::getSize() {
    return size;
}

CodeListNode* CodeList::getFirst() {
    return first;
}

void CodeList::clear() {
    first = nullptr;
    size = 0;
}

void CodeList::printCodeList() {
    if (!first) {
        throw runtime_error("Empty code list");
    }

    CodeListNode* itr = first;
    cout << "PRINTING CODES LIST" << endl;
    while (itr) {
        cout << ">> \'" << itr->character << "\': ";
        for (int i = 0; i < itr->binarySize; i++) {
            cout << itr->binaryDigits[i];
        }
        cout << endl;
        itr = itr->next;
    }
}