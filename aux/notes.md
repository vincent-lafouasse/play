# Notes

## Waveform Audio File Format

```
<WAVE-form> ➝
        RIFF( 'WAVE'
              <fmt-ck>             // format chunk
              [<fact-ck>]          // fact chunk (skipped)
              [<cue-ck>]           // cue points (skipped)
              [<playlist-ck>]      // playlist chunk (skipped)
              [<assoc-data-list>]  // associated data list (skipped)
              <wave-data> )
```
