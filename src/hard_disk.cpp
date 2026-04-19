#include "hard_disk.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "block.h"

HardDisk::HardDisk() {
    std::cout << "booting hard disk ...\n";

    std::filesystem::path sourceDir = PROJECT_ROOT_DIR;
    std::filesystem::path resourcesDir = sourceDir / "resources";
    std::filesystem::create_directory(resourcesDir);
    std::filesystem::path file = resourcesDir / "virtual_disk.vhd";

    if (!std::filesystem::exists(file)) {
        std::ofstream ofs(file);
        ofs.close();
    }

    filePointer.open(file, std::ios::out | std::ios::in | std::ios::binary);

    if (filePointer.is_open()) {
        char ch = 'a';
        for (int i = 0; i < 1024; i++) {
            filePointer.write(&ch, sizeof(ch));
        }

        std::cout << "hard disk is ready\n";
    } else {
        std::cerr << "hard disk boot faliure\n";
    }
}

HardDisk::~HardDisk() { filePointer.close(); }

void HardDisk::read(int block_index, unsigned char buffer[], int size) {
    if (block_index >= capacity) return;

    int block_postion = block_index * BLOCK_SIZE;
    filePointer.seekg(block_postion, std::ios::beg);
    filePointer.read((char*)buffer, size);
}

void HardDisk::write(int block_index, unsigned char buffer[], int size) {
    if (block_index >= capacity) return;

    int block_postion = block_index * BLOCK_SIZE;
    filePointer.seekp(block_postion, std::ios::beg);
    filePointer.write((char*)buffer, size);
}
