
#include "streamfs.h"

#include <cstring>
#include <ctime>
#include <iostream>

#include "block.h"
#include "inode.h"

streamfs::streamfs() {
    std::cout << "file system begin setup\n";
    inode* node = new inode();
    node->is_directory = true;
    node->name[0] = '/', node->name[1] = '\0';
    node->uid = 0;
    node->parent = -1;
    node->size = 0;
    node->timestamp = (unsigned long long)time(nullptr);
    std::vector<int> slot = lookup.get_blocks(1);
    node->data = slot[0];
    memset(&fs_tree[slot[0]], 255, sizeof(index_node));
    inodes.push_back(node);

    std::cout << "filesystem setup end\n";
}

streamfs::~streamfs() {
    for (inode* node : inodes) {
        delete node;
    }
}

int streamfs::inode_from_path(std::vector<std::string>& path_token) {
    inode* current = inodes[0];
    int ans = 0;
    for (std::string& path : path_token) {
        bool flag = false;
        int* arr = (int*)&fs_tree[current->data];
        for (int i = 0; i < current->size; i++) {
            if (strcmp(path.c_str(), inodes[arr[i]]->name) == 0) {
                flag = true;
                current = inodes[arr[i]];
                ans = arr[i];
                break;
            }
        }
        if (!flag) return -1;
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
    int inode_pos = inode_from_path(path_token);

    if (inode_pos == -1) return false;

    inode* node = new inode();
    node->is_directory = false;
    node->size = len;
    strcpy(node->name, name.c_str());
    node->parent = inode_pos;
    node->timestamp = (unsigned long long)time(nullptr);
    node->uid = 0;

    int* arr = (int*)&fs_tree[inode_pos];
    for (int i = 0; i < 4; i++) {
        if (arr[i] == -1) {
            arr[i] = inodes.size();
            break;
        }
        if (i == 3) {
            delete node;
            std::cout << "limit exceeds\n";
            return false;
        }
    }

    inodes[inode_pos]->size += 1;
    inodes.emplace_back(node);

    return true;
}

int streamfs::open(std::string path) {
    std::vector<std::string> path_token = path_tokenizer(path);
    return inode_from_path(path_token);
}

int streamfs::get_block(int file_descriptor, int index) {
    return fs_tree[inodes[file_descriptor]->data].index;
}

int streamfs::read(int file_descriptor, unsigned char buffer[], int offset) {
    int index = (offset + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int block_index = get_block(file_descriptor, index);
    int read_len = BLOCK_SIZE;

    if (block_index == inodes[file_descriptor]->size / BLOCK_SIZE) {
        read_len = inodes[file_descriptor]->size % BLOCK_SIZE;
    }

    hard_disk.read(block_index, buffer, read_len);

    return read_len;
}

int streamfs::write(int file_descriptor, unsigned char buffer[], int offset) {
    int index = (offset + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int block_index = get_block(file_descriptor, index);
    int write_len = BLOCK_SIZE;

    if (block_index == inodes[file_descriptor]->size / BLOCK_SIZE) {
        write_len = inodes[file_descriptor]->size % BLOCK_SIZE;
    }

    hard_disk.write(block_index, buffer, write_len);

    return write_len;
}

int streamfs::ls(std::string path) {
    std::vector<std::string> path_token = path_tokenizer(path);
    int index = inode_from_path(path_token);
    int count = 0;

    int* arr = (int*)&fs_tree[inodes[index]->data];
    for (int i = 0; i < 3; i++) {
        if (arr[i] != -1) {
            count++;
        }
    }

    return count;
}
