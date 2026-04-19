#include <iostream>

#include "streamfs.h"

void print_vector(std::vector<std::string>& arr) {
    std::cout << "[";
    for (std::string& str : arr) {
        std::cout << str << ", ";
    }
    std::cout << "]\n";
}

int main(int argc, char* argv[]) {
    std::cout << "stream-fs is started\n";
    streamfs filesystem;
    std::vector<std::string> temp;

    filesystem.create_directory("/", "dir1");
    temp = filesystem.ls("/");
    print_vector(temp);

    filesystem.create("/", "file1.txt");
    temp = filesystem.ls("/");
    print_vector(temp);

    filesystem.create_directory("/", "dir2");
    temp = filesystem.ls("/");
    print_vector(temp);
    filesystem.create_directory("/", "dir3");

    filesystem.create("/dir1", "def.txt");
    temp = filesystem.ls("/dir1");
    print_vector(temp);

    temp = filesystem.ls("/dir2");
    print_vector(temp);

    temp = filesystem.ls("/");
    print_vector(temp);
    temp = filesystem.ls("/file1.txt");
    print_vector(temp);

    return 0;
}
