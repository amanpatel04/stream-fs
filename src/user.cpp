#include "user.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "streamfs.h"

User::User() {
    userPath = "/";
    fs = new streamfs();
}

User::~User() {}

bool User::mkdir(std::string name) {
    if (name.length() > 30) {
        return false;
    }

    return fs->create_directory(userPath, name);
}

bool User::cd(std::string path) {
    if (path.empty()) return false;
    path += "/";
    std::string tempPath;
    if (path[0] == '/') {
        tempPath = path;
    } else {
        tempPath = userPath + path;
    }
    int result = fs->validPath(tempPath);
    if (result == 1) {
        userPath = tempPath;
        return true;
    }
    return false;
}

bool User::uploadFile(std::string file) {
    std::filesystem::path sourceDir = PROJECT_ROOT_DIR;
    std::filesystem::path filePath = sourceDir / "resources" / file;

    if (std::filesystem::is_regular_file(filePath)) {
        int32_t size = std::filesystem::file_size(filePath);
        bool destFile = fs->create(userPath, file, size);
        int destFileDesc = fs->open(userPath + file);

        if (!destFileDesc || destFileDesc == -1) {
            std::cout << "unable to upload file\n";
            return false;
        }

        char* buffer = new char[BLOCK_SIZE];
        std::size_t bufferLen = 0;
        std::size_t writtenLen = 0;

        std::ifstream fin(filePath, std::ios::binary);

        while (fin.read(buffer, BLOCK_SIZE)) {
            std::size_t bufferLen = fin.gcount();

            fs->write(destFileDesc, buffer, writtenLen);
            writtenLen += bufferLen;
        }

        if (fin.gcount() > 0) {
            fs->write(destFileDesc, buffer, writtenLen);
        }
        delete[] buffer;
        fin.close();

        return true;
    } else {
        std::cout << "Invalid file\n";
    }
    return false;
}

int User::open(std::string file) {
    int result = fs->validPath(userPath + file);
    if (result == 0) {
        int fileDescriptor = fs->open(userPath + file);
        if (openFileTable.find(fileDescriptor) != openFileTable.end()) {
            std::cout << "file already open\n";
            return -1;
        }
        uint64_t size = fs->getNodeSize(fileDescriptor);
        openFileTable.emplace(fileDescriptor, FileData(0, size));
        return fileDescriptor;
    } else {
        std::cout << "unable to locate file\n";
    }
    return -1;
}

void User::read(int fileDescriptor) {
    std::unordered_map<int, FileData>::iterator itr;
    itr = openFileTable.find(fileDescriptor);
    if (itr == openFileTable.end()) {
        std::cout << "file is not opened\n";
        return;
    }
    FileData& fileData = itr->second;
    std::string str;
    char* buffer = new char[BLOCK_SIZE];
    while (fileData.pointer < fileData.size) {
        int readLen = fs->read(fileDescriptor, buffer, fileData.pointer);
        fileData.pointer += readLen;
        std::cout << fileData.pointer << " " << fileData.size << "\n";
    }
    delete[] buffer;
}

void User::close(int fileDescriptor) {
    std::unordered_map<int, FileData>::iterator itr;
    itr = openFileTable.find(fileDescriptor);
    if (itr != openFileTable.end()) {
        openFileTable.erase(fileDescriptor);
    }
}
