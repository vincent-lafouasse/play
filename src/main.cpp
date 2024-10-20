#include <iostream>

#include "Track.h"

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

    Track track(av[1]);
}
