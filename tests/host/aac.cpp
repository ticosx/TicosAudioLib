#include <Arduino.h>
#include "AudioSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorAAC.h"

#define AAC "../../examples/PlayAACFromPROGMEM/homer.aac"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    AudioSourceSTDIO *in = new AudioSourceSTDIO(AAC);
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("out.aac.wav");
    void *space = malloc(28000+60000);
    AudioGeneratorAAC *aac = new AudioGeneratorAAC(space, 28000+60000);

    aac->begin(in, out);
    while (aac->loop()) { /*noop*/ }
    aac->stop();

    delete aac;
    delete out;
    delete in;

    free(space);
}
