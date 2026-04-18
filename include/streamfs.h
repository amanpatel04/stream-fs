#pragma once
#include <array>
#include <string>

#include "indexing.h"
#include "inode.h"

class streamfs {
    std::array<index_node, table_size> fs_tree;
    table lookup;
    std::vector<inode*> inodes;

   public:
    streamfs();
    ~streamfs();
    bool create(const char* name);
    int open(std::string path);
    int read(int file_descriptor, unsigned char buffer[], int size);
    void read_video(int file_descriptor, unsigned char buffer[], int offset);
    int write(int file_descriptor, unsigned char buffer[], int size);
};
