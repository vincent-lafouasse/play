#include "Track.h"
#include <cassert>
#include <cstdint>
#include <iostream>

#include "LittleEndianReader.h"

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

static MetaData read_metadata(LittleEndianReader& reader);

Track::Track(const std::string& path) {
    LittleEndianReader reader(path);

    MetaData metadata = read_metadata(reader);
    assert(metadata.bit_depth == 8 || metadata.bit_depth == 16 ||
           metadata.bit_depth == 24 || metadata.bit_depth == 32);
    assert(metadata.n_channels <= 2);
    metadata.log();

    assert(reader.peek_fourcc() == "data");
}

static MetaData read_format_chunk(LittleEndianReader& reader);
static void skip_chunk_until(LittleEndianReader& reader,
                             const std::string& fourcc);

static MetaData read_metadata(LittleEndianReader(&reader)) {
    assert(reader.read_fourcc() == "RIFF");
    reader.advance(4);  // discard chunk size
    assert(reader.read_fourcc() == "WAVE");

    skip_chunk_until(reader, "fmt ");
    MetaData metadata = read_format_chunk(reader);
    skip_chunk_until(reader, "data");
    return metadata;
}

static MetaData read_format_chunk(LittleEndianReader& reader) {
    MetaData metadata = {};

    assert(reader.read_fourcc() == "fmt ");
    uint32_t fmt_chunk_sz = reader.read_u32();  // either 16, 18 or 40
    assert(fmt_chunk_sz == 16 || fmt_chunk_sz == 18 || fmt_chunk_sz == 40);
    assert(reader.read_u16() == WAVE_FORMAT_PCM);
    metadata.n_channels = reader.read_u16();
    metadata.sample_rate = reader.read_u32();
    reader.advance(4);  // discard data rate
    reader.advance(2);  // discard block size
    metadata.bit_depth = reader.read_u16();

    if (fmt_chunk_sz > 16)
        reader.advance(2);
    if (fmt_chunk_sz > 18)
        reader.advance(22);

    return metadata;
}

static void skip_chunk_until(LittleEndianReader& reader,
                             const std::string& fourcc) {
    while (reader.peek_fourcc() != fourcc) {
        reader.advance(4);  // discard fourcc
        uint32_t size = reader.read_u32();
        reader.advance(size);
    }
    assert(reader.peek_fourcc() == fourcc);
}
