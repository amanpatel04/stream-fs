#pragma once

#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>

struct InodeTreeNode {
    int32_t left;
    int32_t data;
    int32_t right;
    int32_t height;
    char name[32];

    InodeTreeNode() {
        left = right = data = -1;
        height = 0;
    }
};

struct Inode {
    uint64_t size;
    uint64_t timestamp;

    int32_t parent;
    int32_t uid;
    int32_t data;

    uint8_t is_directory;
    uint8_t reserved[3];

    char name[32];

    Inode(bool is_directory, std::string name, int32_t parent,
          uint64_t size = 0, int32_t uid = 0) {
        this->is_directory = is_directory;
        strcpy(this->name, name.c_str());
        this->parent = parent;
        this->uid = uid;
        this->size = size;
        this->timestamp = (unsigned long long)time(nullptr);
        this->data = -1;
    }
};

class InodeTree {
    const int32_t N = 1000;
    std::vector<InodeTreeNode> inodeTree;
    std::vector<int> freeList;

   public:
    InodeTree();
    ~InodeTree();
    int insert(int root, int key, const char* name);
    int getInode(int root, const char* name);
    std::vector<std::string> getInodeNames(int root);
};
