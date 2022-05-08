/*
  AudioRecorderSourceI2S
  I2S source for recorder.
  
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

#ifndef _AudioRecorderSourceI2S_H
#define _AudioRecorderSourceI2S_H

#include <Arduino.h>
#include "AudioRecorderSource.h"

class AudioRecorderSourceI2S : public AudioRecorderSource
{

  public:
    AudioRecorderSourceI2S();
    virtual ~AudioRecorderSourceI2S() override;
    
    //Due to Arduino I2S API definition, it will be 2 channels for always
    virtual bool open(uint16_t hertz, uint8_t bps, uint8_t channels) override;
    virtual uint32_t read(void *data, uint32_t len) override;
    virtual uint32_t readNonBlock(void *data, uint32_t len) override;
    virtual bool seek(int32_t pos, int dir) override;
    virtual bool close() override;
    virtual bool isOpen() override;

  protected:
    bool i2sOn = false;
};

#endif

