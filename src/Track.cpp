#include "Track.h"
#include <cassert>
#include <cstdint>
#include <fstream>
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
static std::vector<std::vector<float>> read_data(LittleEndianReader& reader,
                                                 MetaData metata);

Track::Track(const std::string& path) : m_data() {
    LittleEndianReader reader(path);

    MetaData metadata = read_metadata(reader);
    assert(metadata.bit_depth == 8 || metadata.bit_depth == 16 ||
           metadata.bit_depth == 24 || metadata.bit_depth == 32);
    assert(metadata.n_channels <= 2);

    assert(reader.peek_fourcc() == "data");
    m_data = read_data(reader, metadata);
    m_sampling_rate = metadata.sample_rate;
}

static float read_sample(LittleEndianReader& reader, uint32_t bit_depth);

static std::vector<std::vector<float>> read_data(LittleEndianReader& reader,
                                                 MetaData metadata) {
    assert(reader.peek_fourcc() == "data");
    std::vector<std::vector<float>> out{};
    size_t n_blocks =
        reader.read_u32() / (metadata.n_channels * metadata.bit_depth / 8);

    out.push_back(std::vector<float>());
    if (metadata.n_channels == 2)
        out.push_back(std::vector<float>());

    std::ofstream raw_data("raw_data.tsv");

    for (size_t _ = 0; _ < n_blocks; _++) {
        for (size_t channel = 0; channel < metadata.n_channels; channel++) {
            if (metadata.bit_depth == 24) {
                int32_t sample = reader.read_i24();
                int32_t i24_max = (1 << 23) - 1;

                if (sample > i24_max)
                    sample = i24_max;
                if (sample < -i24_max)
                    sample = -i24_max;
                float converted_sample =
                    static_cast<float>(sample) / static_cast<float>(i24_max);
                out[channel].push_back(converted_sample);
                raw_data << converted_sample << '\t';
            } else {
                std::cerr << "unrecognized bit depth: " << metadata.bit_depth
                          << std::endl;
                std::exit(1);
            }
        }
    }

    return out;
}

static float read_sample(LittleEndianReader& reader, uint32_t bit_depth) {
    if (bit_depth == 24) {
        int32_t sample = reader.read_i24();
        int32_t i24_max = (1 << 23) - 1;

        if (sample > i24_max)
            sample = i24_max;
        if (sample < -i24_max)
            sample = -i24_max;
        return static_cast<float>(sample) / static_cast<float>(i24_max);
    }
    std::cerr << "unrecognized bit depth: " << bit_depth << std::endl;
    std::exit(1);
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

static void write_single_track(const std::vector<float>& samples,
                               const char* name);

void Track::write_tsv() const {
    write_single_track(m_data[0], "data.tsv");
}

static void write_single_track(const std::vector<float>& samples,
                               const char* name) {
    std::ofstream file(name);

    for (float element : samples) {
        file << element << '\t';
    }
}

size_t Track::n_channels() const {
    return m_data.size();
}

size_t Track::sampling_rate() const {
    return m_sampling_rate;
}

size_t Track::len() const {
    return m_data[0].size();
}
