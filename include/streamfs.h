#pragma once
#include <array>
#include <string>

#include "hard_disk.h"
#include "indexing.h"
#include "inode.h"

class streamfs {
    std::array<index_node, table_size> fs_tree;
    table lookup;
    std::vector<inode*> inodes;
    HardDisk hard_disk;

    int inode_from_path(std::vector<std::string>& path_token);
    std::vector<std::string> path_tokenizer(std::string path);
    int get_block(int file_descriptor, int index);

   public:
    streamfs();
    ~streamfs();
    bool create(std::string path, std::string name, int len = 0);
    int open(std::string path);
    int read(int file_descriptor, unsigned char buffer[], int offset);
    int write(int file_descriptor, unsigned char buffer[], int offset);
};
