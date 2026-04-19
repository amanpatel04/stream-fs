#include <gtest/gtest.h>

#include <set>
#include <vector>

#include "indexing.h"

/*
===========================================================
Fixture: IndexingFixture
- Provides a fresh 'table' object for every test
- Ensures tests are independent
===========================================================
*/

class IndexingFixture : public ::testing::Test {
   protected:
    table lookup;
};

// --------------------------------------------------------
// Test 1: Allocate full capacity
// --------------------------------------------------------
TEST_F(IndexingFixture, AllocateFullCapacity) {
    std::vector<int> blocks = lookup.get_blocks(20000);

    ASSERT_EQ(blocks.size(), 20000);

    std::set<int> unique(blocks.begin(), blocks.end());
    EXPECT_EQ(unique.size(), 20000);
}

// --------------------------------------------------------
// Test 2: Exhaustion (no blocks left)
// --------------------------------------------------------
TEST_F(IndexingFixture, Exhaustion) {
    lookup.get_blocks(20000);

    std::vector<int> result = lookup.get_blocks(1);

    EXPECT_EQ(result.size(), 0);
}

// --------------------------------------------------------
// Test 3: Free and reuse blocks
// --------------------------------------------------------
TEST_F(IndexingFixture, FreeAndReuse) {
    std::vector<int> blocks = lookup.get_blocks(20000);

    // Take last 10 blocks
    std::vector<int> last10(blocks.end() - 10, blocks.end());

    lookup.add_blocks(last10);

    // Request more than available
    std::vector<int> fail = lookup.get_blocks(11);
    EXPECT_EQ(fail.size(), 0);

    // Request exact available
    std::vector<int> reused = lookup.get_blocks(10);
    EXPECT_EQ(reused.size(), 10);

    // Verify reused blocks are from freed ones
    std::set<int> freed(last10.begin(), last10.end());

    for (size_t i = 0; i < reused.size(); i++) {
        EXPECT_TRUE(freed.count(reused[i]));
    }

    // No blocks left again
    std::vector<int> empty = lookup.get_blocks(1);
    EXPECT_EQ(empty.size(), 0);
}

// --------------------------------------------------------
// Test 4: Zero request
// --------------------------------------------------------
TEST_F(IndexingFixture, ZeroRequest) {
    std::vector<int> result = lookup.get_blocks(0);

    EXPECT_EQ(result.size(), 0);
}

// --------------------------------------------------------
// Test 5: Partial allocation
// --------------------------------------------------------
TEST_F(IndexingFixture, PartialAllocation) {
    std::vector<int> blocks1 = lookup.get_blocks(100);
    EXPECT_EQ(blocks1.size(), 100);

    std::vector<int> blocks2 = lookup.get_blocks(19900);
    EXPECT_EQ(blocks2.size(), 19900);

    std::vector<int> fail = lookup.get_blocks(1);
    EXPECT_EQ(fail.size(), 0);
}

// --------------------------------------------------------
// Test 6: Invalid free (should not corrupt system)
// --------------------------------------------------------
TEST_F(IndexingFixture, InvalidFree) {
    std::vector<int> invalid;
    invalid.push_back(-1);
    invalid.push_back(30000);

    lookup.add_blocks(invalid);

    std::vector<int> blocks = lookup.get_blocks(20000);
    EXPECT_EQ(blocks.size(), 20000);
}

// --------------------------------------------------------
// Test 7: Double free detection
// --------------------------------------------------------
TEST_F(IndexingFixture, DoubleFree) {
    std::vector<int> blocks = lookup.get_blocks(5);

    lookup.add_blocks(blocks);
    lookup.add_blocks(blocks);  // double free

    std::vector<int> result = lookup.get_blocks(20000);

    std::set<int> unique(result.begin(), result.end());

    EXPECT_EQ(unique.size(), result.size());
}

// --------------------------------------------------------
// Test 8: Interleaved operations
// --------------------------------------------------------
TEST_F(IndexingFixture, InterleavedOperations) {
    std::vector<int> a = lookup.get_blocks(100);
    std::vector<int> b = lookup.get_blocks(200);

    EXPECT_EQ(a.size(), 100);
    EXPECT_EQ(b.size(), 200);

    lookup.add_blocks(a);

    std::vector<int> c = lookup.get_blocks(50);
    EXPECT_EQ(c.size(), 50);

    std::vector<int> remaining = lookup.get_blocks(20000);

    std::set<int> unique(remaining.begin(), remaining.end());
    EXPECT_EQ(unique.size(), remaining.size());
}
