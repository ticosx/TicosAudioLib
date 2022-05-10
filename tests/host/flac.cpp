#include <Arduino.h>
#include "AudioSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorFLAC.h"

#define AAC "gs-16b-2c-44100hz.flac"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    AudioSourceSTDIO *in = new AudioSourceSTDIO(AAC);
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("out.flac.wav");
    AudioGeneratorFLAC *flac = new AudioGeneratorFLAC();

    flac->begin(in, out);
    while (flac->loop()) { /*noop*/ }
    flac->stop();

    delete flac;
    delete out;
    delete in;
}
