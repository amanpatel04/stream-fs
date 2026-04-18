#pragma once

#include <cstddef>

struct inode {
    bool is_directory;
    char name[16];
    int parent;
    int uid;
    std::size_t size;
    unsigned long long timestamp;
    int data;
};
