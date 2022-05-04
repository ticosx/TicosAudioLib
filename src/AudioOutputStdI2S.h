/*
  AudioOutputStdI2S
  Base class for an I2S output port, which is defined at https://docs.arduino.cc/learn/built-in-libraries/i2s
  
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

#pragma once

#include "AudioOutput.h"

class AudioOutputStdI2S : public AudioOutput
{
  public:
#if defined(ESP32) || defined(ESP8266)
    AudioOutputStdI2S();
    bool setAllPins(int sckPin, int fsPin, int sdPin, int outSdPin, int inSdPin);
#endif
    virtual ~AudioOutputStdI2S() override;
    virtual bool setRate(int hz) override;
    virtual bool setBitsPerSample(int bits) override;
    virtual bool setChannels(int channels) override;
    virtual bool begin() override;
    virtual bool consumeSample(int16_t sample[2]) override;
    virtual void flush() override;
    virtual bool stop() override;
    virtual bool loop() override;

  protected:
    bool i2sOn;
    uint8_t *buffer = NULL;
    int16_t dataIndex = 0;
    size_t bufferLength = 0;
    size_t transferData();

};
