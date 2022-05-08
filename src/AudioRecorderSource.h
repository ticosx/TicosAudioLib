/*
  AudioRecorderSource
  Base class of a recorder source
  
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

#ifndef _AUDIORECORDERSOURCE_H
#define _AUDIORECORDERSOURCE_H

#include <Arduino.h>
#include "AudioFileSource.h"

class AudioRecorderSource : public AudioFileSource
{

  public:
    AudioRecorderSource(){}
    virtual ~AudioRecorderSource() override {};
    
    virtual bool open(uint16_t hertz, uint8_t bps, uint8_t channels) {this->hertz=hertz;this->bps=bps;this->channels=channels;return true;};

  protected:
    uint16_t hertz;
    uint8_t bps;
    uint8_t channels;
};

#endif

