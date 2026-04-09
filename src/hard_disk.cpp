#include "hard_disk.h"

#include <filesystem>
#include <fstream>
#include <iostream>

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
