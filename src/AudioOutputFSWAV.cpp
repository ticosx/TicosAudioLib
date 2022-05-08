/*
  AudioOutputFSWAV
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

#include <Arduino.h>
#include <FS.h>

#include "AudioOutputFSWAV.h"

static const uint8_t wavHeaderTemplate[] PROGMEM = { // Hardcoded simple WAV header with 0xffffffff lengths all around
    0x52, 0x49, 0x46, 0x46, 0xff, 0xff, 0xff, 0xff, 0x57, 0x41, 0x56, 0x45,
    0x66, 0x6d, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x22, 0x56, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x04, 0x00, 0x10, 0x00,
    0x64, 0x61, 0x74, 0x61, 0xff, 0xff, 0xff, 0xff };

void AudioOutputFSWAV::setFilename(FS& fs, const char *name)
{
  if (filename) free(filename);
  filesystem = &fs;
  filename = strdup(name);
}

bool AudioOutputFSWAV::begin()
{
  uint8_t wavHeader[sizeof(wavHeaderTemplate)];
  memset(wavHeader, 0, sizeof(wavHeader));

  if (f) return false; // Already open!
  filesystem->remove(filename);
  f = filesystem->open(filename, "w+");
  if (!f) return false;

#ifdef ESP32
  buff = (uint8_t*) ps_malloc(buffLen);
#else
  buff = (uint8_t*) malloc(buffLen);
  if(!buff){
    error("AudioGeneratorRecorder", "Cannot allocate buffer");
  }
#endif
  dataLen = 0;
  
  // We'll fix the header up when we close the file
  f.write(wavHeader, sizeof(wavHeader));
  return true;
}

void AudioOutputFSWAV::flush()
{
  if(dataLen > 0){
    f.write(buff, dataLen);
  }
  dataLen = 0;
}

bool AudioOutputFSWAV::consumeSample(int16_t sample[2])
{
  for (int i=0; i<channels; i++) {
    if (bps == 8) {
      uint8_t l = sample[i] & 0xff;
      buff[dataLen++] = l;
      // f.write(&l, sizeof(l));
    } else {
      uint8_t l = sample[i] & 0xff;
      uint8_t h = (sample[i] >> 8) & 0xff;
      buff[dataLen++] = l;
      buff[dataLen++] = h;
      // f.write(&l, sizeof(l));
      // f.write(&h, sizeof(h));
    }
  }
  if(dataLen>=buffLen){
    flush();
  }
  return true;
}

// uint16_t AudioOutputFSWAV::consumeSamples(int16_t *samples, uint16_t count)
// {
//   return f.write((uint8_t *)samples, (size_t)(count * (bps / 8) * channels));
// }


bool AudioOutputFSWAV::stop()
{
  uint8_t wavHeader[sizeof(wavHeaderTemplate)];

  flush();

  memcpy_P(wavHeader, wavHeaderTemplate, sizeof(wavHeaderTemplate));

  int chunksize = f.size() - 8;
  wavHeader[4] = chunksize & 0xff;
  wavHeader[5] = (chunksize>>8)&0xff;
  wavHeader[6] = (chunksize>>16)&0xff;
  wavHeader[7] = (chunksize>>24)&0xff;

  wavHeader[22] = channels & 0xff;
  wavHeader[23] = 0;

  wavHeader[24] = hertz & 0xff;
  wavHeader[25] = (hertz >> 8) & 0xff;
  wavHeader[26] = (hertz >> 16) & 0xff;
  wavHeader[27] = (hertz >> 24) & 0xff;
  int byteRate = hertz * bps * channels / 8;
  wavHeader[28] = byteRate & 0xff;
  wavHeader[29] = (byteRate >> 8) & 0xff;
  wavHeader[30] = (byteRate >> 16) & 0xff;
  wavHeader[31] = (byteRate >> 24) & 0xff;
  wavHeader[32] = channels * bps / 8;
  wavHeader[33] = 0;
  wavHeader[34] = bps;
  wavHeader[35] = 0;

  int datasize = f.size() - sizeof(wavHeader);
  wavHeader[40] = datasize & 0xff;
  wavHeader[41] = (datasize>>8)&0xff;
  wavHeader[42] = (datasize>>16)&0xff;
  wavHeader[43] = (datasize>>24)&0xff;

  // Write real header out
  f.seek(0, SeekSet);
  f.write(wavHeader, sizeof(wavHeader));
  f.close();

  if(buff){
    free(buff);
  }
  buff = NULL;
  return true;
}
 
