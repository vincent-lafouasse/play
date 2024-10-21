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

struct MetaData {
    uint16_t n_channels;
    uint32_t sample_rate;
    uint32_t bit_depth;

    void log() const {
        std::cout << "n channels " << sample_rate << "\nn_channels "
                  << n_channels << "\nbit depth " << bit_depth << std::endl;
    }
};

static MetaData read_metadata(Bytes& bytes, size_t& at);
static MetaData read_format_chunk(Bytes& bytes, size_t& at);
static void skip_chunk_until(Bytes& bytes,
                             size_t& at,
                             const std::string& fourcc);

Track::Track(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::in);
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<uint8_t> bytes(std::istreambuf_iterator<char>(input), {});
    size_t index = 0;

    MetaData metadata = read_metadata(bytes, index);
    assert(LittleEndian::peek_fourcc(bytes, index) == "data");
}

static MetaData read_metadata(Bytes& bytes, size_t& at) {
    assert(LittleEndian::read_fourcc(bytes, at) == "RIFF");
    at += 4;  // discard chunk size
    assert(LittleEndian::read_fourcc(bytes, at) == "WAVE");

    skip_chunk_until(bytes, at, "fmt ");
    MetaData metadata = read_format_chunk(bytes, at);
    assert(metadata.bit_depth == 8 || metadata.bit_depth == 16 ||
           metadata.bit_depth == 24 || metadata.bit_depth == 32);
    metadata.log();

    skip_chunk_until(bytes, at, "data");
    return metadata;
}

static MetaData read_format_chunk(Bytes& bytes, size_t& at) {
    MetaData metadata = {};

    assert(LittleEndian::read_fourcc(bytes, at) == "fmt ");
    uint32_t fmt_chunk_sz =
        LittleEndian::read_u32(bytes, at);  // either 16, 18 or 40
    assert(fmt_chunk_sz == 16 || fmt_chunk_sz == 18 || fmt_chunk_sz == 40);
    assert(LittleEndian::read_u16(bytes, at) == WAVE_FORMAT_PCM);
    metadata.n_channels = LittleEndian::read_u16(bytes, at);
    metadata.sample_rate = LittleEndian::read_u32(bytes, at);
    at += 4;  // discard data rate
    at += 2;  // discard block size
    metadata.bit_depth = LittleEndian::read_u16(bytes, at);

    if (fmt_chunk_sz > 16)
        at += 2;
    if (fmt_chunk_sz > 18)
        at += 22;

    return metadata;
}

static void skip_chunk_until(Bytes& bytes,
                             size_t& at,
                             const std::string& fourcc) {
    while (LittleEndian::peek_fourcc(bytes, at) != fourcc) {
        at += 4;  // discard fourcc
        uint32_t size = LittleEndian::read_u32(bytes, at);
        at += size;
    }
    assert(LittleEndian::peek_fourcc(bytes, at) == fourcc);
}
