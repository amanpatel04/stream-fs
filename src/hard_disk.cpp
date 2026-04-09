
#include "hard_disk.h"

HardDisk::HardDisk() {
    std::cout << "booting hard disk ...\n";
    filePointer.open("../resources/virtual_disk.vhd",
                     std::ios::out | std::ios::in | std::ios::binary);
    if (filePointer.is_open()) {
        char ch = 'a';
        for (int i = 0; i < 1024; i++) {
            filePointer.write(&ch, sizeof(ch));
        }

        std::cout << "hard disk is ready\n";
    } else {
        std::cout << "hard disk boot faliure\n";
    }
}

HardDisk::~HardDisk() { filePointer.close(); }
