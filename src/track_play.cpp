#include <portaudio.h>
#include <iostream>
#include "Track.h"

#define BUFFER_SIZE 256

struct MonoPlaybackContext {
    const std::vector<float>* samples;
    size_t i;
};

struct StreamConfig {
    int n_in;
    int n_out;
    double sample_rate;
    unsigned long buffer_size;
};

int callback(const void* _input_buffer,
             void* output_buffer,
             unsigned long buffer_size,
             const PaStreamCallbackTimeInfo* _time_info,
             PaStreamCallbackFlags _status_flags,
             void* user_data) {
    (void)_input_buffer;
    (void)_time_info;
    (void)_status_flags;

    float* buffer = static_cast<float*>(output_buffer);
    MonoPlaybackContext* ctx = static_cast<MonoPlaybackContext*>(user_data);

    for (size_t i = 0; i < buffer_size; i++) {
        if (ctx->i < ctx->samples->size()) {
            buffer[i] = ctx->samples->at(ctx->i);
            ctx->i++;
        } else {
            buffer[i] = 0.0;
        }
    }

    return 0;
}

static void check_error(PaError err);

void Track::play() const {
    StreamConfig cfg = {0, 1, static_cast<double>(this->sampling_rate()),
                        BUFFER_SIZE};
    MonoPlaybackContext ctx = {&m_data[0], 0};

    PaStream* stream;
    check_error(Pa_OpenDefaultStream(&stream, cfg.n_in, cfg.n_out, paFloat32,
                                     cfg.sample_rate, cfg.buffer_size, callback,
                                     &ctx));
}

static void check_error(PaError err) {
    if (err != paNoError) {
        std::cerr << Pa_GetErrorText(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
