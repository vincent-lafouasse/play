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
        out += (bytes[at + i] << i);

    at += 2;
    return out;
}

[[maybe_unused]] uint32_t read_u32(const Bytes& bytes, size_t& at) {
    assert(at + 4 < bytes.size());

    uint16_t out{};
    for (size_t i = 0; i < 2; i++)
        out += (bytes[at + i] << i);

    at += 4;
    return out;
}

[[maybe_unused]] uint64_t read_u64(const Bytes& bytes, size_t& at) {
    assert(at + 8 < bytes.size());

    uint64_t out{};
    for (size_t i = 0; i < 8; i++)
        out += (bytes[at + i] << i);

    at += 8;
    return out;
}

[[maybe_unused]] std::string read_fourcc(const Bytes& bytes, size_t& at) {
    assert(at + 4 < bytes.size());

    char buffer[5] = {0};
    std::memcpy(buffer, &bytes[at], 4);

    at += 4;
    return std::string(buffer);
}
}  // namespace LittleEndian
