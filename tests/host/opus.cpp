#include <Arduino.h>
#include "AudioSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorOpus.h"

#define OPUS "../../examples/PlayOpusFromSPIFFS/data/gs-16b-2c-44100hz.opus"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    AudioSourceSTDIO *file = new AudioSourceSTDIO(OPUS);
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("opus.wav");
    AudioGeneratorOpus *opus = new AudioGeneratorOpus();

    opus->begin(file, out);
    while (opus->loop()) { /*noop*/ }
    opus->stop();

    delete out;
    delete opus;
    delete file;
}
