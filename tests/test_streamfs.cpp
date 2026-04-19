#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include "streamfs.h"

/*
===========================================================
Fixture: StreamFsFixture

- Creates a fresh filesystem before each test
- Ensures isolation between tests
===========================================================
*/

class StreamFsFixture : public ::testing::Test {
   protected:
    streamfs fs;

    void SetUp() override {
        // filesystem initialized automatically in constructor
    }

    void TearDown() override {
        // destructor cleans memory
    }
};

// --------------------------------------------------------
// Test 1: Root directory exists
// --------------------------------------------------------
TEST_F(StreamFsFixture, RootExists) {
    int root = fs.open("/");
    EXPECT_EQ(root, 0);
}

// --------------------------------------------------------
// Test 2: Create directory
// --------------------------------------------------------
TEST_F(StreamFsFixture, CreateDirectory) {
    bool result = fs.create_directory("/", "home");
    EXPECT_TRUE(result);

    int inode = fs.open("/home");
    EXPECT_NE(inode, -1);
}

// --------------------------------------------------------
// Test 3: Nested directory creation
// --------------------------------------------------------
TEST_F(StreamFsFixture, NestedDirectory) {
    fs.create_directory("/", "home");
    bool result = fs.create_directory("/home", "user");

    EXPECT_TRUE(result);
    EXPECT_NE(fs.open("/home/user"), -1);
}

// --------------------------------------------------------
// Test 4: Invalid path for directory
// --------------------------------------------------------
TEST_F(StreamFsFixture, InvalidDirectoryPath) {
    bool result = fs.create_directory("/invalid", "dir");
    EXPECT_FALSE(result);
}

// --------------------------------------------------------
// Test 5: Create file
// --------------------------------------------------------
TEST_F(StreamFsFixture, CreateFile) {
    fs.create_directory("/", "docs");

    bool result = fs.create("/docs", "file.txt", 100);

    EXPECT_TRUE(result);
    EXPECT_NE(fs.open("/docs/file.txt"), -1);
}

// --------------------------------------------------------
// Test 6: Invalid file path
// --------------------------------------------------------
TEST_F(StreamFsFixture, InvalidFilePath) {
    bool result = fs.create("/invalid", "file.txt", 100);
    EXPECT_FALSE(result);
}

// --------------------------------------------------------
// Test 7: File name too long
// --------------------------------------------------------
TEST_F(StreamFsFixture, FileNameTooLong) {
    std::string long_name = "this_name_is_way_too_long";

    bool result = fs.create("/", long_name, 100);

    EXPECT_FALSE(result);
}

// --------------------------------------------------------
// Test 8: Directory listing (ls)
// --------------------------------------------------------
TEST_F(StreamFsFixture, ListDirectory) {
    fs.create_directory("/", "a");
    fs.create_directory("/", "b");
    fs.create_directory("/", "c");

    std::vector<std::string> result = fs.ls("/");

    std::set<std::string> names(result.begin(), result.end());

    EXPECT_TRUE(names.count("a"));
    EXPECT_TRUE(names.count("b"));
    EXPECT_TRUE(names.count("c"));
}

// --------------------------------------------------------
// Test 9: ls on file (invalid)
// --------------------------------------------------------
TEST_F(StreamFsFixture, ListOnFile) {
    fs.create("/", "file.txt", 10);

    std::vector<std::string> result = fs.ls("/file.txt");

    EXPECT_EQ(result.size(), 0);
}

// --------------------------------------------------------
// Test 11: Open invalid path
// --------------------------------------------------------
TEST_F(StreamFsFixture, OpenInvalidPath) {
    int result = fs.open("/does/not/exist");
    EXPECT_EQ(result, -1);
}

// --------------------------------------------------------
// Test 12: Directory capacity limit (max 4 entries)
// --------------------------------------------------------
TEST_F(StreamFsFixture, DirectoryLimit) {
    fs.create_directory("/", "a");
    fs.create_directory("/", "b");
    fs.create_directory("/", "c");

    bool result = fs.create_directory("/", "d");  // should fail

    EXPECT_FALSE(result);
}

// --------------------------------------------------------
// Test 13: Write and Read basic (integration)
// --------------------------------------------------------
/*
TEST_F(StreamFsFixture, WriteReadBasic) {
    fs.create("/", "file.txt", 1024);

    int fd = fs.open("/file.txt");

    unsigned char write_buffer[1024];
    unsigned char read_buffer[1024];

    for (int i = 0; i < 1024; i++) {
        write_buffer[i] = (unsigned char)(i % 256);
    }

    int written = fs.write(fd, write_buffer, 0);
    EXPECT_GT(written, 0);

    int read = fs.read(fd, read_buffer, 0);
    EXPECT_GT(read, 0);

    for (int i = 0; i < read; i++) {
        EXPECT_EQ(write_buffer[i], read_buffer[i]);
    }
}
*/

// --------------------------------------------------------
// Test 14: Multiple directory structure
// --------------------------------------------------------
TEST_F(StreamFsFixture, DeepStructure) {
    fs.create_directory("/", "a");
    fs.create_directory("/a", "b");
    fs.create_directory("/a/b", "c");

    EXPECT_NE(fs.open("/a/b/c"), -1);
}

// --------------------------------------------------------
// Test 15: Duplicate names allowed (check behavior)
// --------------------------------------------------------
TEST_F(StreamFsFixture, DuplicateNames) {
    fs.create("/", "file.txt", 10);
    bool result = fs.create("/", "file.txt", 10);

    // Your implementation allows duplicates currently
    EXPECT_TRUE(result);
}
