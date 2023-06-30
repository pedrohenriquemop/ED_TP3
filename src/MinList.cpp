#include "MinList.hpp"

MinList::MinList() {
    first = nullptr;
    size = 0;
}

void MinList::add(char c) {
    if (!first) {
        first = new Node(c, 1);
        size++;
        return;
    }

    Node* itr = first;
    while (itr) {
        if (itr->character == c) {
            itr->freq++;
            return;
        }
        if (itr->next) {
            itr = itr->next;
        } else {
            itr->next = new Node(c, 1);
            size++;
            return;
        }
    }
};

Node* MinList::popMin() {
    if (!first) {
        throw runtime_error("Empty list");
    }

    int min = findMinFreq();
    Node* found;

    if (first->freq == min) {
        found = first;
        first = first->next;
        size--;
        return found;
    }

    Node* itr = first;
    while (itr) {
        if (itr->next->freq == min) {
            found = itr->next;
            itr->next = itr->next->next;
            size--;
            return found;
        }
        itr = itr->next;
    }
    throw runtime_error("Minimun frequency character not found");
}

int MinList::getSize() {
    return size;
}

int MinList::findMinFreq() {
    if (!first) {
        throw runtime_error("Empty list");
    }

    Node* itr = first;
    int min = itr->freq;
    while ((itr = itr->next)) {
        if (itr->freq < min) min = itr->freq;
    }

    return min;
}

void MinList::print() {
    Node* itr = first;

    while (itr) {
        cout << "| \'" << itr->character << "\' " << itr->freq << endl;
        itr = itr->next;
    }
}