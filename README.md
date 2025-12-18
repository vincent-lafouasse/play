# play

a WAV decoder and player using the `portaudio` audio interface and standard C++17

this became [cPlayer](https://github.com/vincent-lafouasse/cPlayer)

## Build

Build by running these from the root of the repository
```sh
cmake -B build -S .
cmake --build build
```

You will find the binary in `./build`

## Usage

```sh
./play WAV_FILE
```

## references

- https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
- Multimedia Programming Interface and Data Specifications 1.0 p56
