/*
  AudioGeneratorRecorder
  Audio recorder
  
  Copyright (C) 2022 Tiwater

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AudioGeneratorRecorder_H
#define _AudioGeneratorRecorder_H

#include "AudioGenerator.h"
#include "AudioRecorderSource.h"

class AudioGeneratorRecorder : public AudioGenerator
{
  public:
    AudioGeneratorRecorder();
    AudioGeneratorRecorder(uint16_t hertz, uint8_t bps, uint8_t channels);
    virtual ~AudioGeneratorRecorder() override;
    virtual bool begin(AudioRecorderSource *source, AudioOutput *output);
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override;

  protected:
    uint16_t hertz;
    uint8_t bps;
    uint8_t channels;

    // Input buffering
    int buffLen = 1024;
    uint8_t *buff = NULL;
    int16_t buffValid;
    int16_t lastFrameEnd;
    bool FillBufferWithValidFrame(); 

    // Output buffering
    int16_t *outSample; // Interleaved L/R
    int16_t validSamples;
    int16_t curSample;

    // Each frame may change this if they're very strange, I guess
    unsigned int lastRate;
    int lastChannels;

};

#endif

