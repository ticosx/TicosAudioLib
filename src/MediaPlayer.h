/*
  MediaPlayer
  Support playing media
  
  Copyright (C) 2022  Tiwater Ltd.

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

#ifndef MediaPlayer_H
#define MediaPlayer_H

#include <Arduino.h>
#include "audiohandler/AudioGenerator.h"
#include "audiohandler/AudioSource.h"
#include "audiohandler/AudioOutput.h"

typedef enum {
    STOPPED = 0,
    PLAYING = 1,
    PAUSED  = 2,
} media_state;

class MediaPlayer
{
public:
  MediaPlayer();
  ~MediaPlayer();

public:
  bool play(const char* res);
  bool stop();
  bool pause();
  bool resume();
  bool loop();
  media_state getState();

protected:
  AudioGenerator *audio;
  media_state state = STOPPED;
  AudioSource *source;
  AudioOutput *output;
};

#endif
