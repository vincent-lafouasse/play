#pragma once

#include <vector>

class Track {
   public:
    Track(const std::string& path);
    void play() const;

    size_t n_channels() const;
    size_t sampling_rate() const;
    size_t len() const;
    void write_tsv() const;

   private:
    std::vector<std::vector<float>> m_data;
    size_t m_sampling_rate;
};
