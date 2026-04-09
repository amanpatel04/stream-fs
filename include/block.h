#pragma once

#include <cstdint>

#define BLOCK_SIZE 65536

struct Block {
    uint8_t data[BLOCK_SIZE];
};
