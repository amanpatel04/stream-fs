
#include "streamfs.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

streamfs::streamfs() {
    std::cout << "file system begin setup\n";

    Inode* node = new Inode(true, "/", -1);
    inodes.push_back(node);

    std::cout << "filesystem setup end\n";
}

streamfs::~streamfs() {
    for (Inode* node : inodes) {
        delete node;
    }
}

int streamfs::inode_from_path(std::vector<std::string>& path_token) {
    int ans = 0;
    for (std::string& path : path_token) {
        ans = inodeTree.getInode(ans, path.c_str());
        if (ans == -1) return -1;
    }
    return ans;
}

std::vector<std::string> streamfs::path_tokenizer(std::string path) {
    std::vector<std::string> path_token;
    std::string temp = "";
    for (int i = 0, n = path.length(); i < n; i++) {
        if (path[i] == '/') {
            if (temp != "") {
                path_token.push_back(temp);
            }
            temp = "";
        } else {
            temp += path[i];
        }
    }
    if (temp != "") path_token.push_back(temp);
    return path_token;
}

bool streamfs::create(std::string path, std::string name, int len) {
    if (name.length() > 15) return false;

    std::vector<std::string> path_token = path_tokenizer(path);
    int parentInode = inode_from_path(path_token);

    if (parentInode == -1) return false;

    int blockRequire = (len + BLOCK_SIZE - 1) / BLOCK_SIZE;

    Inode* node = new Inode(false, name, parentInode, len);
    node->data = avlTree.balloc(blockRequire);

    inodes[parentInode]->data = inodeTree.insert(inodes[parentInode]->data,
                                                 inodes.size(), name.c_str());
    inodes.emplace_back(node);
    inodes[parentInode]->size += 1;

    return true;
}

int streamfs::open(std::string path) {
    std::vector<std::string> path_token = path_tokenizer(path);
    return inode_from_path(path_token);
}

int streamfs::get_block(int file_descriptor, int index) {
    return avlTree.getAt(inodes[file_descriptor]->data, index);
}

int streamfs::read(int file_descriptor, char buffer[], int offset) {
    int index = offset / BLOCK_SIZE;
    int block_index = get_block(file_descriptor, index);
    int read_len = BLOCK_SIZE;

    if (index == inodes[file_descriptor]->size / BLOCK_SIZE) {
        read_len = inodes[file_descriptor]->size % BLOCK_SIZE;
    }

    hard_disk.read(block_index, buffer, read_len);

    return read_len;
}

int streamfs::write(int file_descriptor, char buffer[], int offset) {
    int index = offset / BLOCK_SIZE;
    int block_index = get_block(file_descriptor, index);
    int write_len = BLOCK_SIZE;

    if (block_index == inodes[file_descriptor]->size / BLOCK_SIZE) {
        write_len = inodes[file_descriptor]->size % BLOCK_SIZE;
    }

    hard_disk.write(block_index, buffer, write_len);

    return write_len;
}

bool streamfs::create_directory(std::string path, std::string name) {
    if (name.length() > 15) return false;

    std::vector<std::string> path_token = path_tokenizer(path);
    int parentInode = inode_from_path(path_token);

    if (parentInode == -1) return false;

    Inode* node = new Inode(true, name, parentInode);

    inodes[parentInode]->data = inodeTree.insert(inodes[parentInode]->data,
                                                 inodes.size(), name.c_str());

    inodes.emplace_back(node);
    inodes[parentInode]->size += 1;

    return true;
}

std::vector<std::string> streamfs::ls(std::string path) {
    std::vector<std::string> path_token = path_tokenizer(path);
    int index = inode_from_path(path_token);

    if (!inodes[index]->is_directory || index == -1) {
        std::cout << "target is not directory\n";
        return {};
    }

    std::vector<std::string> result =
        inodeTree.getInodeNames(inodes[index]->data);

    return result;
}

uint64_t streamfs::getNodeSize(int nodeIndex) {
    return inodes[nodeIndex]->size;
}

int streamfs::validPath(std::string path) {
    std::vector<std::string> path_token = path_tokenizer(path);
    int index = inode_from_path(path_token);
    if (index == -1) {
        return -1;
    }
    return inodes[index]->is_directory;
}
