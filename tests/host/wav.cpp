#include <Arduino.h>
#include "AudioSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorWAV.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    AudioSourceSTDIO *in = new AudioSourceSTDIO("test_8u_16.wav");
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("pcm.wav");
    AudioGeneratorWAV *wav = new AudioGeneratorWAV();

    wav->begin(in, out);
    while (wav->loop()) { /*noop*/ }
    wav->stop();

    delete wav;
    delete out;
    delete in;
}
