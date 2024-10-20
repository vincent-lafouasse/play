#include "Track.h"
#include <cstdint>
#include <fstream>
#include <iostream>

#include "read.h"

Track::Track(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::in);
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<uint8_t> bytes(std::istreambuf_iterator<char>(input), {});
    size_t index = 0;

    std::cout << LittleEndian::read_fourcc(bytes, index) << std::endl;
    std::cout << LittleEndian::read_u32(bytes, index) << std::endl;
    std::cout << LittleEndian::read_fourcc(bytes, index) << std::endl;
    std::cout << LittleEndian::read_fourcc(bytes, index) << std::endl;
    std::cout << LittleEndian::read_u32(bytes, index) << std::endl;
    std::cout << LittleEndian::read_u16(bytes, index) << std::endl;
    std::cout << LittleEndian::read_u16(bytes, index) << std::endl;
    std::cout << LittleEndian::read_u32(bytes, index) << std::endl;
    std::cout << LittleEndian::read_u32(bytes, index) << std::endl;
}
