#include "read.h"

namespace LittleEndian {
[[maybe_unused]] uint8_t read_u8(std::istream is);
[[maybe_unused]] uint16_t read_u16(std::istream is);
[[maybe_unused]] uint32_t read_u32(std::istream is);
[[maybe_unused]] uint64_t read_u64(std::istream is);
[[maybe_unused]] std::string read_fourcc(std::istream is);
}  // namespace LittleEndian
