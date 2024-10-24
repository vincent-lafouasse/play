#include <iostream>

#include "Track.h"

#define USAGE "play WAV_FILE"

int main(int ac, char* av[]) {
    if (ac != 2) {
        std::cerr << USAGE << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Track track(av[1]);
    track.write_tsv();

    uint32_t duration_ms = 1000.0 * static_cast<float>(track.len()) / static_cast<float>(track.sampling_rate());
    std::cout << duration_ms << std::endl;
    // track.play();
}
