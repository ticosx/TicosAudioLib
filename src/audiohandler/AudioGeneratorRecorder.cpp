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

#include "AudioGeneratorRecorder.h"
#include "Log.h"

AudioGeneratorRecorder::AudioGeneratorRecorder():AudioGeneratorRecorder(44100, 16, 2)
{
}

AudioGeneratorRecorder::AudioGeneratorRecorder(uint16_t hertz, uint8_t bps, uint8_t channels)
{

  this->hertz = hertz;
  this->bps = bps;
  this->channels = channels;

  buffLen = 400 * bps / 8 * 2;

  running = false;
  buffValid = 0;
  lastFrameEnd = 0;
  validSamples = 0;
  curSample = 0;
  lastRate = 0;
  lastChannels = 0;
}



AudioGeneratorRecorder::~AudioGeneratorRecorder()
{
  if(!buff)
  {
    free(buff);
  }
}

bool AudioGeneratorRecorder::stop()
{
  if(running){
    running = false;
    validSamples = 0;
    curSample = 0;
    if(buff){
      free(buff);
    }
    buff = NULL;
    outSample = NULL;
    output->stop();
    return source->close();
  }
  return true;
}

bool AudioGeneratorRecorder::isRunning()
{
  return running;
}

bool AudioGeneratorRecorder::FillBufferWithValidFrame()
{
  uint32_t readLen = source->readNonBlock(buff, buffLen);
  //Input must be stero
  validSamples = readLen / (bps / 8) / 2;
  curSample = 0;
  return true;
}

bool AudioGeneratorRecorder::loop()
{
  if (!running) goto out; // Nothing to do here!

  // No samples available, need to decode a new frame
  while (FillBufferWithValidFrame()) {
    bool needBreak = false;
    if(validSamples != buffLen / (bps / 8) / 2){
      //
      needBreak = true;
    }
      // If we've got data, try and pump it out...
    while (validSamples) {
      if (bps == 8) {
        if (channels == 1) {
          lastSample[0] = buff[curSample*2];
          lastSample[1] = buff[curSample*2];
        } else {
          lastSample[0] = buff[curSample*2];
          lastSample[1] = buff[curSample*2 + 1];
        }
      } else {
        if (channels == 1) {
          lastSample[0] = outSample[curSample*2];
          lastSample[1] = outSample[curSample*2];
        } else {
          lastSample[0] = outSample[curSample*2];
          lastSample[1] = outSample[curSample*2 + 1];
        }
      }
      if (!output->consumeSample(lastSample)) goto done; // Can't send, but no error detected
      validSamples--;
      curSample++;
    } 
    
    if(needBreak)
      break;
  }

done:
  source->loop();
  output->loop();
out:

  return running;
}

bool AudioGeneratorRecorder::begin(AudioRecorderSource *source, AudioOutput *output)
{
  if (!source) return false;
  this->source = source;
  if (!output) return false;
  this->output = output;
  source->open(hertz, bps, channels);
  if (!source->isOpen()) return false; // Error

#ifdef ESP32
    buff = (uint8_t*) ps_malloc(buffLen);
#else
    buff = (uint8_t*) malloc(buffLen);
    if(!buff){
      error("AudioGeneratorRecorder", "Cannot allocate buffer");
    }
#endif
  outSample = (int16_t*) buff;

  if (!output->setRate( hertz )) {
    Serial.printf_P(PSTR("AudioGeneratorRecorder::begin: failed to setRate in output\n"));
    return false;
  }
  if (!output->setBitsPerSample( bps )) {
    Serial.printf_P(PSTR("AudioGeneratorRecorder::begin: failed to setBitsPerSample in output\n"));
    return false;
  }
  if (!output->setChannels( channels )) {
    Serial.printf_P(PSTR("AudioGeneratorRecorder::begin: failed to setChannels in output\n"));
    return false;
  }
  output->begin();

  memset(buff, 0, buffLen);

  validSamples = 0;
  curSample = 0;
 
  running = true;
  
  return true;
}


