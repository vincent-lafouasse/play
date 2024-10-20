#pragma once

#include <vector>

class Track {
   public:
    Track(const std::string& path);
    void play() const;

    size_t n_channels() const;
    size_t sampling_rate() const;

   private:
    std::vector<std::vector<float>> data;
};
