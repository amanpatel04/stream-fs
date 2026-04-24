#include <iostream>

#include "user.h"

int main(int argc, char* argv[]) {
    std::cout << "stream-fs is started\n";

    User user;
    user.mkdir("dir1");
    user.cd("dir1");
    bool result = user.uploadFile("source.txt");
    if (result) {
        std::cout << "File read\n";
        user.cd("/");
        int descp = user.open("source.txt");
        user.read(descp);
        user.cd("dir1");
        descp = user.open("source.txt");
        user.read(descp);

    } else {
        std::cout << "Something went wrong\n";
    }

    return 0;
}
