
#include "streamfs.h"

#include <ctime>

streamfs::streamfs() {
    inode* node = new inode();
    node->is_directory = true;
    node->name[0] = '/', node->name[1] = '\0';
    node->uid = 0;
    node->parent = -1;
    node->size = 0;
    node->timestamp = (unsigned long long)ctime(NULL);
    std::vector<int> slot = lookup.get_blocks(1);
    node->data = slot[0];
    inodes.push_back(node);
}

streamfs::~streamfs() {
    for (inode* node : inodes) {
        delete node;
    }
}
