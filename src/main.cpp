#include <cstdint>
#include <fstream>
#include <iostream>

#include "read.h"

#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE

#define USAGE "play WAV_FILE"

int main(int ac, char* av[]) {
    if (ac != 2) {
        std::cerr << USAGE << std::endl;
        std::exit(EXIT_FAILURE);
    }

    const char* path = av[1];
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
