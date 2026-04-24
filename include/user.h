#pragma once

#include <cstdint>
#include <unordered_map>

#include "streamfs.h"

struct FileData {
    int32_t pointer;
    uint64_t size;
    FileData(int32_t pointer, uint64_t size) {
        this->pointer = pointer;
        this->size = size;
    }
};

class User {
    streamfs* fs;
    std::string userPath;
    std::unordered_map<int, FileData> openFileTable;

   public:
    User();
    ~User();
    bool mkdir(std::string name);
    bool cd(std::string path);

    bool uploadFile(std::string file);
    int open(std::string file);
    void read(int fileDescriptor);
    void close(int fileDescriptor);
};
