#include "MediaRecorder.h"
#include "audiohandler/AudioOutputFSWAV.h"
#include "audiohandler/AudioRecorderSourceI2S.h"
#include "audiohandler/AudioGeneratorRecorder.h"
#include "FS.h"
#include "SPIFFS.h"
#include "SD_MMC.h"
#include "Log.h"

MediaRecorder::MediaRecorder()
{
}

MediaRecorder::~MediaRecorder()
{
  //Ensure release the resources
  stop();
}

bool MediaRecorder::record(const char *res)
{
  if (recording)
  {
    // Not stopped, then stop the media playing now
    stop();
  }

  fs::FS *fs;
  //TODO: Check file location
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

  // TODO: check the resource type
  String resName = String(res);
  resName.toLowerCase();
  if (resName.endsWith(".wav"))
  {
    // WAV
    // TODO: split FS and WAV
    output = new AudioOutputFSWAV();
  } else {
    //Other type
  }
  output->setFilename(*fs, res);
  //Record source is I2S
  source = new AudioRecorderSourceI2S();

  //The recorder to transfer data from source to output
  audio = new AudioGeneratorRecorder();
  recording = true;
  return audio->begin(source, output);
}

bool MediaRecorder::stop()
{
  if (recording && audio)
  {
    audio->stop();
    if (source)
    {
      // clear source
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
    recording = false;
  }
  return true;
}

bool MediaRecorder::loop()
{
  if(recording){
    bool running = audio->loop();
    if(!running){
      //Playing end
      stop();
    }
    return running;
  } else {
    return false;
  }
}

bool MediaRecorder::isRecording()
{
  return recording;
}
