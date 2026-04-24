#pragma once

#include <cstdint>
#include <vector>

struct AvlTreeNode {
    int32_t left, right, size;
    AvlTreeNode() {
        left = -1;
        right = -1;
        size = 0;
    }
};

class AvlTree {
    const int N = 16384;
    std::vector<AvlTreeNode> blockTree;
    std::vector<int> freeList;

    std::vector<int> get_blocks(int size);
    void add_blocks(std::vector<int>& blocks);
    int buildBlockList(std::vector<int>& arr, int start, int end);

   public:
    AvlTree();
    ~AvlTree();
    int balloc(int size);
    int getSize(int root);
    int getAt(int root, int index);
};
