#include "MediaPlayer.h"
#include "audiohandler/AudioSourceFS.h"
#include "audiohandler/AudioGeneratorAAC.h"
#include "audiohandler/AudioGeneratorWAV.h"
#include "audiohandler/AudioOutputStdI2S.h"
#include "FS.h"
#include "SPIFFS.h"
#include "SD_MMC.h"
#include "Log.h"

MediaPlayer::MediaPlayer()
{
}

MediaPlayer::~MediaPlayer()
{
  //Ensure release the resources
  stop();
}

bool MediaPlayer::play(const char *res)
{
  if (state != STOPPED)
  {
    // Not stopped, then stop the media playing now
    stop();
  }
  // TODO: check the audio type
  String resName = String(res);
  resName.toLowerCase();
  if (resName.endsWith(".aac"))
  {
    // AAC
    audio = new AudioGeneratorAAC();
  }
  else if (resName.endsWith(".wav"))
  {
    // WAV
    audio = new AudioGeneratorWAV();
  }

  // TODO: check the resource type
  if (true)
  {
    // File
    fs::FS *fs;
    // TODO: Check file location
    if (true)
    {
      if (!SD_MMC.begin())
      {
        logErr("SD_MMC mount failed");
        return false;
      }
      fs = &SD_MMC;
    }
    else if (true)
    {
      if (!SPIFFS.begin())
      {
        logErr("SPIFFS mount failed");
        return false;
      }
      fs = &SPIFFS;
    }
    
    source = new AudioSourceFS(*fs, res);
  }
  else if (true)
  {
    // HTTP Stream
  }

  //MediaPlayer always output from I2S. Extend here if want to support more
  output = new AudioOutputStdI2S();
  state = PLAYING;
  return audio->begin(source, output);
}

bool MediaPlayer::stop()
{
  if (state != STOPPED && audio)
  {
    audio->stop();
    if (source)
    {
      // Close and clear source
      source->close();
      delete (source);
      source = NULL;
    }
    if (output)
    {
      // Close and clear output
      delete (output);
      output = NULL;
    }
    // Clear audio
    delete (audio);
    audio = NULL;
    state = STOPPED;
  }
  return true;
}

bool MediaPlayer::pause()
{
  if(state == PLAYING){
    //Only meaningful in playing state
    state = PAUSED;
    return true;
  }
  return false;
}

bool MediaPlayer::resume()
{
  if(state == PAUSED){
    //Resume play when paused
    state = PLAYING;
    return true;
  }
  return false;
}

bool MediaPlayer::loop()
{
  if(state == PLAYING){
    bool end = audio->loop();
    if(!end){
      //Playing end
      stop();
    }
    return end;
  } else if(state == PAUSED){
    //Do nothing is pause
    return true;
  } else {
    return false;
  }
}

media_state MediaPlayer::getState()
{
  return state;
}
