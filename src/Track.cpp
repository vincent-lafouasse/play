#include "Track.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "read.h"

#define WAVE_FORMAT_PCM 0x0001
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE

template <typename T>
void log_var(const T& e) {
    std::cout << e << std::endl;
}

Track::Track(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::in);
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<uint8_t> bytes(std::istreambuf_iterator<char>(input), {});
    size_t index = 0;

    assert(LittleEndian::read_fourcc(bytes, index) == "RIFF");
    LittleEndian::read_u32(bytes, index);  // discard chunk size
    assert(LittleEndian::read_fourcc(bytes, index) == "WAVE");

    assert(LittleEndian::read_fourcc(bytes, index) == "fmt ");
    uint32_t fmt_chunk_sz = LittleEndian::read_u32(bytes, index);
    log_var<uint32_t>(fmt_chunk_sz);
    assert(LittleEndian::read_u16(bytes, index) == WAVE_FORMAT_PCM);
    uint16_t n_channels = LittleEndian::read_u16(bytes, index);
    log_var<uint16_t>(n_channels);
    uint32_t blocks_per_sec = LittleEndian::read_u32(bytes, index);
    uint32_t data_rate = LittleEndian::read_u32(bytes, index);
    uint16_t block_size = LittleEndian::read_u32(bytes, index);
    uint16_t sample_size = LittleEndian::read_u32(bytes, index);
}
