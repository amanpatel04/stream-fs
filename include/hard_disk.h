#pragma once

#include <fstream>

// #include "block.h"

class HardDisk {
   private:
    int currentHead = 0;
    int capacity = 16384;
    std::fstream filePointer;

   public:
    HardDisk();
    ~HardDisk();
    void read(int block_index, unsigned char buffer[], int size);
    void write(int block_index, unsigned char buffer[], int size);
};
