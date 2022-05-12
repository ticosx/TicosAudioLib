/*
  AudioRecorderSourceI2S
  MIC source
  
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
#include "AudioRecorderSourceI2S.h"
#include <I2S.h>
#include <stdexcept>
//Depends on https://github.com/ticosx/TicosService
#include "ServiceManager.h"
#include "AudioService.h"
#include "Log.h"
#define ADUIO_RECORD_SRC_MIC_TAG "AudioRecorderSourceI2S"

AudioRecorderSourceI2S::AudioRecorderSourceI2S()
{
#ifdef ESP32
  //setAllPins(PIN_I2S_SCK, PIN_I2S_FS, PIN_I2S_SD, PIN_I2S_SD_OUT, PIN_I2S_SD_IN);
  if(!I2S.setDuplex()){
    error(ADUIO_RECORD_SRC_MIC_TAG, "Could not set duplex");
    throw std::runtime_error("Invalid service type");
  }
#endif
  AudioService* audioService = (AudioService*)ServiceManager::getService(AUDIO_SERVICE);
  if(audioService == NULL) {
    warn(ADUIO_RECORD_SRC_MIC_TAG, "No AudioService defined");
  } else {
    codec = audioService->getAudioAdpater();
  }
}

bool AudioRecorderSourceI2S::open(uint16_t hertz, uint8_t bps, uint8_t channels)
{
  AudioRecorderSource::open(hertz, bps, channels);
  
  if(i2sOn){
    return true;
  }

  if (!I2S.begin(I2S_PHILIPS_MODE, hertz, bps)) {
    error(ADUIO_RECORD_SRC_MIC_TAG, "Failed to initialize I2S!");
    return false;
  }
  if(codec){
    codec->setBitsPerSample(bps);
    codec->start(AUDIO_HAL_CODEC_MODE_ENCODE);
  }
  i2sOn = true;
  return true;
}

AudioRecorderSourceI2S::~AudioRecorderSourceI2S()
{
  close();
}

uint32_t AudioRecorderSourceI2S::read(void *data, uint32_t len)
{
  if(i2sOn)
  {
    if (data==NULL) {
      audioLogger->printf_P(PSTR("ERROR! AudioRecorderSourceI2S::read passed NULL data\n"));
      return 0;
    }
    //Always stero
    return I2S.read(data, len);
  }
  else
  {
    return 0;
  }
}

uint32_t AudioRecorderSourceI2S::readNonBlock(void *data, uint32_t len)
{
  if(i2sOn)
  {
    if (data==NULL) {
      audioLogger->printf_P(PSTR("ERROR! AudioRecorderSourceI2S::readNonBlock passed NULL data\n"));
      return 0;
    }
    uint32_t toRead = I2S.available();
    if(toRead>len)
    {
      toRead = len;
    }
    if(toRead > 0){
      //Always stero
      return I2S.read(data, toRead);
    } else {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

bool AudioRecorderSourceI2S::seek(int32_t pos, int dir)
{
  audioLogger->printf_P(PSTR("ERROR! AudioRecorderSourceI2S::seek not implemented!"));
  (void) pos;
  (void) dir;
  return false;
}

bool AudioRecorderSourceI2S::close()
{

  if(codec){
    codec->stop(AUDIO_HAL_CODEC_MODE_ENCODE);
  }
  if(i2sOn){
    I2S.end();
    i2sOn = false;
  }
  return true;
}

bool AudioRecorderSourceI2S::isOpen()
{
  return i2sOn;
}


