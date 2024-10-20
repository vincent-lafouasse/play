#include "read.h"
#include <cassert>

namespace LittleEndian {
[[maybe_unused]] uint8_t read_u8(const Bytes& bytes, size_t& at) {
    assert(at + 1 < bytes.size());
    return bytes[at++];
}

[[maybe_unused]] uint16_t read_u16(const Bytes& bytes, size_t& at) {
    assert(at + 2 < bytes.size());

    uint16_t out{};
    for (size_t i = 0; i < 2; i++)
        out += (bytes[at + i] << 8 * i);

    at += 2;
    return out;
}

[[maybe_unused]] uint32_t read_u32(const Bytes& bytes, size_t& at) {
    assert(at + 4 < bytes.size());

    uint16_t out{};
    for (size_t i = 0; i < 2; i++)
        out += (bytes[at + i] << 8 * i);

    at += 4;
    return out;
}

[[maybe_unused]] uint64_t read_u64(const Bytes& bytes, size_t& at) {
    assert(at + 8 < bytes.size());

    uint64_t out{};
    for (size_t i = 0; i < 8; i++)
        out += (bytes[at + i] << 8 * i);

    at += 8;
    return out;
}

[[maybe_unused]] std::string read_fourcc(const Bytes& bytes, size_t& at) {
    assert(at + 4 < bytes.size());
    std::string out = LittleEndian::peek_fourcc(bytes, at);
    at += 4;
    return out;
}

[[maybe_unused]] std::string peek_fourcc(const Bytes& bytes, size_t& at) {
    assert(at + 4 < bytes.size());

    char buffer[5] = {0};
    std::memcpy(buffer, &bytes[at], 4);
    return std::string(buffer);
}

[[maybe_unused]] int32_t read_i24(const Bytes& bytes, size_t& at) {
    assert(at + 3 < bytes.size());

    uint8_t byte1 = bytes[at];  // LSD
    uint8_t byte2 = bytes[at + 1];
    uint8_t byte3 = bytes[at + 2];  // MSD

    // most significant byte contains sign information on top bit
    bool is_negative = byte3 & 0b10000000;  // top bit
    byte3 = byte3 & 0b01111111;

    int32_t out = byte1 + (byte2 << 8) + (byte3 << 16);  // read as u24
    if (is_negative)
        out -= (1 << 23);  // rectify is top bit is lit

    at += 3;
    return out;
}
}  // namespace LittleEndian
