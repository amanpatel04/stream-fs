#include "indexing.h"

table::table() {
    free_list.resize(table_size);
    for (int i = 0; i < table_size; i++) {
        free_list[i] = i;
    }
}

std::vector<int> table::get_blocks(int size) {
    if (size > free_list.size()) {
        return {};
    }
    std::vector<int> free_blocks(size);
    int n = free_list.size() - 1;
    for (int i = 0; i < size; i++) {
        free_blocks[i] = free_list[n - i];
    }
    free_list.erase(free_list.end() - size, free_list.end());
    return free_blocks;
}

void table::add_blocks(std::vector<int>& blocks) {
    if (free_list.size() + blocks.size() > table_size) {
        return;
    }

    int N = blocks.size();
    for (int i = 0; i < N; i++) {
        free_list.emplace_back(blocks[i]);
    }
}
