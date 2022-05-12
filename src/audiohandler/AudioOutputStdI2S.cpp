/*
  AudioOutputStdI2S
  Base class for I2S interface port
  
  Copyright (C) 2017  Earle F. Philhower, III

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

#include <Arduino.h>
#include <I2S.h>
#include <stdexcept>
#include "AudioOutputStdI2S.h"
//Depends on https://github.com/ticosx/TicosService
#include "ServiceManager.h"
#include "AudioService.h"
#include "Log.h"

#define AUDIO_OUTPUT_STD_I2S_TAG "AudioOutputStdI2S"

AudioOutputStdI2S::AudioOutputStdI2S()
{

  AudioService* audioService = (AudioService*)ServiceManager::getService(AUDIO_SERVICE);
  if(audioService == NULL) {
    warn(AUDIO_OUTPUT_STD_I2S_TAG, "No AudioService defined");
  } else {
    codec = audioService->getAudioAdpater();
  }
  //Default value
  bps = 16;
  channels = 2;
  hertz = 44100;
#ifdef ESP32
  //setAllPins(PIN_I2S_SCK, PIN_I2S_FS, PIN_I2S_SD, PIN_I2S_SD_OUT, PIN_I2S_SD_IN);
  if(!I2S.setDuplex()){
    error(AUDIO_OUTPUT_STD_I2S_TAG, "Could not set duplex");
    throw std::runtime_error("Invalid service type");
  }
#endif
  SetGain(1.0);
}

bool AudioOutputStdI2S::setAllPins(int sckPin, int fsPin, int sdPin, int outSdPin, int inSdPin)
{
  return I2S.setAllPins(sckPin, fsPin, sdPin, outSdPin, inSdPin);
}

AudioOutputStdI2S::~AudioOutputStdI2S()
{
  stop();
  i2sOn = false;
}

bool AudioOutputStdI2S::setRate(int hz)
{
  // TODO - have a list of allowable rates from constructor, check them
  this->hertz = hz;
  if (i2sOn)
  {
    //Change the paratemter need to stop and begin I2S again
    stop();
  }
  return true;
}

bool AudioOutputStdI2S::setBitsPerSample(int bits)
{
  if ( (bits != 16) && (bits != 8) ) return false;
  this->bps = bits;
  if (i2sOn)
  {
    if(codec){
      codec->setBitsPerSample(bits);
    }
    //Change the paratemter need to stop and begin I2S again
    stop();
  }
  return true;
}

bool AudioOutputStdI2S::setChannels(int channels)
{
  if ( (channels < 1) || (channels > 2) ) return false;
  if (channels == 1) {
    warn(AUDIO_OUTPUT_STD_I2S_TAG, "I2S is fixed 2 channels");
  }
  this->channels = channels;
  return true;
}

bool AudioOutputStdI2S::loop()
{
  //If it's stopped by the parameter setting
  if(!i2sOn)
  {
    //Auto start it again
    begin();
  }
  return true;
}

bool AudioOutputStdI2S::begin()
{
  if(i2sOn){
    return true;
  }

  if (!I2S.begin(I2S_PHILIPS_MODE, hertz, bps)) {
    error(AUDIO_OUTPUT_STD_I2S_TAG, "Failed to initialize I2S!");
    return false;
  }
  if(codec){
    codec->start(AUDIO_HAL_CODEC_MODE_DECODE);
  }
  if(!buffer)
  {
    //Malloc buffer
    bufferLength = I2S.getBufferSize() * (bps / 8) * 2; //At least now Arduino I2S API dones't support set channels, so fixed 2 here
#ifdef ESP32
    buffer = (uint8_t*) ps_malloc(bufferLength);
#else
    buffer = (uint8_t*) malloc(bufferLength);
#endif
    if(!buffer)
    {
      error(AUDIO_OUTPUT_STD_I2S_TAG, "Failed to alloc buffer!");
    }
  }
  i2sOn = true;
  return true;
}

bool AudioOutputStdI2S::consumeSample(int16_t sample[2])
{

  //return if we haven't called ::begin yet
  if (!i2sOn)
    return false;

  if(buffer)
  {

    //Data full
    if(dataIndex >= bufferLength)
    {
      //Can send out data
      if(I2S.availableForWrite()>=bufferLength)
      {
        transferData();
      }
      else
      {
        //Cannot handle more data now, return directly
        return false;
      }
    }
    //Put new data into buffer
    if(bps == 8){

      uint8_t ms[2];

      ms[0] = sample[0] & 0xff;
      ms[1] = sample[1] & 0xff;
      
      memcpy(&buffer[dataIndex], ms, 2);
      dataIndex += 2;
    } else {
      int16_t ms[2];

      ms[0] = sample[0];
      ms[1] = sample[1];
      
      memcpy(&buffer[dataIndex], ms, 4);
      dataIndex += 4;
    }

    //Sound will be broken without the code below
    if(dataIndex >= bufferLength && I2S.availableForWrite()>=bufferLength)
    {
      //Full, transfer data to I2S
      return transferData();
    }
    return true;
  }
  else
  {
    return false;
  }
}

size_t AudioOutputStdI2S::transferData(){
  size_t ret = I2S.write(buffer, bufferLength);
  dataIndex = 0;
  return ret;
}

void AudioOutputStdI2S::flush()
{
  transferData();
  I2S.flush();
}

bool AudioOutputStdI2S::stop()
{
  if (!i2sOn)
    return false;

  flush();

  if(codec){
    codec->stop(AUDIO_HAL_CODEC_MODE_DECODE);
  }
  I2S.end();
  if(buffer)
  {
    free(buffer);
    buffer = NULL;
    dataIndex = 0;
    bufferLength = 0;
  }
  i2sOn = false;
  return true;
}
