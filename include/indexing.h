#pragma once

#include <vector>

#define table_size 20000

struct index_node {
    int left;
    int index;
    int size;
    int right;

    index_node() {
        left = -1;
        index = -1;
        size = 0;
        right = -1;
    }

    index_node(int left, int index, int size, int right) {
        this->left = left;
        this->index = index;
        this->size = size;
        this->right = right;
    }
};

class table {
   private:
    std::vector<int> free_list;

   public:
    table();
    std::vector<int> get_blocks(int size);
    void add_blocks(std::vector<int>& blocks);
};
