#include <iostream>

#include "streamfs.h"

int main(int argc, char* argv[]) {
    std::cout << "stream-fs is started\n";
    streamfs filesystem;

    std::cout << filesystem.ls("/") << "\n";
    filesystem.create("/", "foo.txt");
    // std::cout << filesystem.ls("/") << "\n";
    // filesystem.create("/", "bar.txt");
    // std::cout << filesystem.ls("/") << "\n";

    return 0;
}
