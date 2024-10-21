#include "OutputStream.h"
#include "Track.h"

#define BUFFER_IZE 256

struct MonoPlaybackContext {
    std::vector<float> samples;
    size_t i;
};

int callback(const void* input_buffer,
             void* output_buffer,
             unsigned long buffer_size,
             const PaStreamCallbackTimeInfo* time_info,
             PaStreamCallbackFlags status_flags,
             void* user_data);

void Track::play() const {}
