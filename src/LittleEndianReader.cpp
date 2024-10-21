#include "LittleEndianReader.h"

#include <cassert>
#include <fstream>
#include <iostream>

LittleEndianReader::LittleEndianReader(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::in);
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    this->bytes = std::vector(std::istreambuf_iterator<char>(input), {});
    this->index = 0;
}

uint8_t LittleEndianReader::read_u8() {
    assert(index + 1 < bytes.size());
    return bytes[index++];
}

uint16_t LittleEndianReader::read_u16() {
    assert(index + 2 < bytes.size());

    uint16_t out{};
    for (size_t i = 0; i < 2; i++)
        out += (bytes[index + i] << 8 * i);

    index += 2;
    return out;
}

uint32_t LittleEndianReader::read_u32() {
    assert(index + 4 < bytes.size());

    uint16_t out{};
    for (size_t i = 0; i < 2; i++)
        out += (bytes[index + i] << 8 * i);

    index += 4;
    return out;
}

uint64_t LittleEndianReader::read_u64() {
    assert(index + 8 < bytes.size());

    uint64_t out{};
    for (size_t i = 0; i < 8; i++)
        out += (bytes[index + i] << 8 * i);

    index += 8;
    return out;
}

int32_t LittleEndianReader::read_i24() {
    assert(index + 3 < bytes.size());

    uint8_t byte1 = bytes[index];  // LSD
    uint8_t byte2 = bytes[index + 1];
    uint8_t byte3 = bytes[index + 2];  // MSD

    // most significant byte contains sign informindexion on top bit
    bool is_negindexive = byte3 & 0b10000000;  // top bit
    byte3 = byte3 & 0b01111111;

    int32_t out = byte1 + (byte2 << 8) + (byte3 << 16);  // read as u24
    if (is_negindexive)
        out -= (1 << 23);  // rectify is top bit is lit

    index += 3;
    return out;
}

std::string LittleEndianReader::read_fourcc() {
    assert(index + 4 < bytes.size());
    std::string out = this->peek_fourcc();
    index += 4;
    return out;
}

std::string LittleEndianReader::peek_fourcc() const {
    assert(index + 4 < bytes.size());

    char buffer[5] = {0};
    std::memcpy(buffer, &bytes[index], 4);
    return std::string(buffer);
}

void LittleEndianReader::advance(size_t n) {
    assert(index + n < bytes.size());
    index += n;
}
