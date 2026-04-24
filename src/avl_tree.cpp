
#include "avl_tree.h"

#include <numeric>

AvlTree::AvlTree() {
    blockTree.resize(N);
    freeList.resize(N);
    std::iota(freeList.begin(), freeList.end(), 0);
}

AvlTree::~AvlTree() {}

int AvlTree::getSize(int root) {
    if (root == -1) {
        return 0;
    }
    return blockTree[root].size;
}

int AvlTree::buildBlockList(std::vector<int>& arr, int start, int end) {
    if (start > end) {
        return -1;
    }
    int mid = start + (end - start) / 2;
    int root = arr[mid];
    blockTree[root].left = buildBlockList(arr, start, mid - 1);
    blockTree[root].right = buildBlockList(arr, mid + 1, end);
    blockTree[root].size =
        getSize(blockTree[root].left) + getSize(blockTree[root].right);
    return root;
}

std::vector<int> AvlTree::get_blocks(int size) {
    if (size > freeList.size()) {
        return {};
    }
    std::vector<int> free_blocks(size);
    int n = freeList.size() - 1;
    for (int i = 0; i < size; i++) {
        free_blocks[i] = freeList[n - i];
    }
    freeList.erase(freeList.end() - size, freeList.end());
    return free_blocks;
}

void AvlTree::add_blocks(std::vector<int>& blocks) {
    if (freeList.size() + blocks.size() > N) {
        return;
    }

    int N = blocks.size();
    for (int i = 0; i < N; i++) {
        freeList.emplace_back(blocks[i]);
    }
}

int AvlTree::balloc(int size) {
    std::vector<int> slots = get_blocks(size);
    if (slots.empty()) return -1;
    return buildBlockList(slots, 0, size - 1);
}

int AvlTree::getAt(int root, int index) {
    if (root == -1) return -1;
    int leftSize = 0;
    int left = blockTree[root].left;
    int right = blockTree[root].right;
    if (left != -1) {
        leftSize = getSize(left);
    }
    if (index < leftSize) {
        return getAt(left, index);
    } else if (index == leftSize) {
        return root;
    } else {
        return getAt(right, index - leftSize - 1);
    }
}
