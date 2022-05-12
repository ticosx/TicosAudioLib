/*
  AudioSourceFS
  Input Arduion "file" to be used by AudioGenerator
  
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

#ifndef _AUDIOSOURCESPIFFS_H
#define _AUDIOSOURCESPIFFS_H

#include <Arduino.h>
#include <FS.h>

#include "AudioSource.h"
#include "AudioSourceFS.h"

class AudioSourceSPIFFS : public AudioSourceFS
{
  public:
    AudioSourceSPIFFS() : AudioSourceFS(SPIFFS) { };
    AudioSourceSPIFFS(const char *filename) : AudioSourceFS(SPIFFS, filename) {};
    // Others are inherited from base
};


#endif

