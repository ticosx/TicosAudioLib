/*
  AudioOutputSPIFFSWAV
  Writes a WAV file to the SPIFFS filesystem
  
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

#ifndef _AUDIOOUTPUTFSWAV_H
#define _AUDIOOUTPUTFSWAV_H

#include <Arduino.h>
#include <FS.h>

#include "AudioOutput.h"

class AudioOutputFSWAV : public AudioOutput
{
  public:
    AudioOutputFSWAV() { filename = NULL; };
    ~AudioOutputFSWAV() { free(filename); };
    virtual bool begin() override;
    virtual bool consumeSample(int16_t sample[2]) override;
    // virtual uint16_t consumeSamples(int16_t *samples, uint16_t count) override;
    virtual bool stop() override;
    virtual void flush() override;
    void setFilename(FS& fs, const char *name);

  private:
    FS *filesystem;
    File f;
    char *filename;
    uint8_t *buff = NULL;
    uint32_t buffLen = 512;
    uint32_t dataLen = 0;

};

#endif

