#pragma once
#include <cstdint>
#include <string>

#include "avl_tree.h"
#include "hard_disk.h"
#include "inode.h"

#define BLOCK_SIZE 65536

class streamfs {
    AvlTree avlTree;
    InodeTree inodeTree;
    std::vector<Inode*> inodes;
    HardDisk hard_disk;

    int inode_from_path(std::vector<std::string>& path_token);
    std::vector<std::string> path_tokenizer(std::string path);
    int get_block(int file_descriptor, int index);

   public:
    streamfs();
    ~streamfs();

    // file operation
    bool create(std::string path, std::string name, int len = 0);
    int open(std::string path);
    int read(int file_descriptor, char buffer[], int offset);
    int write(int file_descriptor, char buffer[], int offset);
    // bool delete(std::string path, std::string name);

    // directory operation
    bool create_directory(std::string path, std::string name);
    // bool delete_directory(std::string path, std::string name);

    // util
    std::vector<std::string> ls(std::string path);
    uint64_t getNodeSize(int nodeIndex);
    int validPath(std::string path);
};
