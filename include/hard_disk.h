#pragma once

#include <fstream>
#include <iostream>

#include "block.h"

class HardDisk {
   private:
    int currentHead = 0;
    int capacity = 16384;
    std::fstream filePointer;

   public:
    HardDisk();
    ~HardDisk();
    void read(int headPointer, Block buffer[], int size);
    void write(int headPointer, Block buffer[], int size);
};
