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
    // track.play();
}
