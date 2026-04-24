
#include "inode.h"

#include <cstring>
#include <iostream>
#include <stack>
#include <vector>

InodeTree::InodeTree() {
    inodeTree.resize(N);
    freeList.resize(N);
    for (int i = N - 1; i >= 0; i--) {
        freeList[N - i - 1] = i;
    }
}

InodeTree::~InodeTree() {}

int InodeTree::insert(int root, int key, const char* name) {
    if (root == -1) {
        int node = freeList.back();
        freeList.pop_back();
        inodeTree[node].data = key;
        strcpy(inodeTree[node].name, name);
        return node;
    }

    int compareValue = strcmp(name, inodeTree[root].name);
    if (compareValue == 0) {
        std::cout << "i-node already exist\n";
        return root;
    } else if (compareValue < 0) {
        inodeTree[root].left = insert(inodeTree[root].left, key, name);
    } else {
        inodeTree[root].right = insert(inodeTree[root].right, key, name);
    }

    return root;
}

int InodeTree::getInode(int root, const char* name) {
    if (root == -1) {
        return -1;
    }
    int compareValue = strcmp(name, inodeTree[root].name);
    if (compareValue == 0) {
        return inodeTree[root].data;
    } else if (compareValue < 0) {
        return getInode(inodeTree[root].left, name);
    } else {
        return getInode(inodeTree[root].right, name);
    }
}

std::vector<std::string> InodeTree::getInodeNames(int root) {
    std::stack<int> stack;
    std::vector<std::string> answer;
    int current = root;

    while (!stack.empty() || current != -1) {
        while (current != -1) {
            stack.push(current);
            current = inodeTree[current].left;
        }

        current = stack.top();
        stack.pop();
        answer.push_back(inodeTree[current].name);
        current = inodeTree[current].right;
    }

    return answer;
}
