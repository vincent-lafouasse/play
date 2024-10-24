#pragma once

#include <cstdint>
#include <string>
#include <vector>

typedef std::vector<char> Bytes;

class LittleEndianReader {
   public:
    LittleEndianReader(const std::string& path);
    void advance(size_t n);
    uint8_t read_u8();
    uint16_t read_u16();
    uint32_t read_u32();
    uint64_t read_u64();
    int64_t read_i24();
    std::string read_fourcc();
    std::string peek_fourcc() const;

   private:
    std::vector<char> bytes;
    size_t index;
};

namespace LittleEndian {
[[maybe_unused]] uint8_t read_u8(const Bytes& bytes, size_t& at);
[[maybe_unused]] uint16_t read_u16(const Bytes& bytes, size_t& at);
[[maybe_unused]] uint32_t read_u32(const Bytes& bytes, size_t& at);
[[maybe_unused]] uint64_t read_u64(const Bytes& bytes, size_t& at);
[[maybe_unused]] int32_t read_i24(const Bytes& bytes, size_t& at);
[[maybe_unused]] std::string read_fourcc(const Bytes& bytes, size_t& at);
[[maybe_unused]] std::string peek_fourcc(const Bytes& bytes, size_t& at);
}  // namespace LittleEndian
