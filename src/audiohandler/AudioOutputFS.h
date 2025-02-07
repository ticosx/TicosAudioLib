/*
  AudioOutputFS
  Support output the media data to filesystem
  
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

#ifndef _AUDIOOUTPUTFS_H
#define _AUDIOOUTPUTFS_H

#include <Arduino.h>
#include <FS.h>

#include "AudioOutput.h"

class AudioOutputFS : public AudioOutput
{
  public:
    AudioOutputFS() {};
    ~AudioOutputFS() { if(filename) free(filename); };
    virtual void setFilename(fs::FS& fs, const char *name){if (filename) free(filename);filesystem = &fs;filename = strdup(name);};

  protected:
    fs::FS *filesystem;
    fs::File f;
    char *filename;

};

#endif

