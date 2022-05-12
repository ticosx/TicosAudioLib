// Lazy "include all the things" header for simplicity.
// In general a user should only include the specific headers they need
// to miniimize build times.

// Input stage
#include "AudioSourceBuffer.h"
#include "AudioSourceFATFS.h"
#include "AudioSourceFS.h"
#include "AudioSource.h"
#include "AudioSourceHTTPStream.h"
#include "AudioSourceICYStream.h"
#include "AudioSourceID3.h"
#include "AudioSourceLittleFS.h"
#include "AudioSourcePROGMEM.h"
#include "AudioSourceSD.h"
#include "AudioSourceSPIFFS.h"
#include "AudioSourceSPIRAMBuffer.h"
#include "AudioSourceSTDIO.h"

// Misc. plumbing
#include "AudioFileStream.h"
#include "AudioLogger.h"
#include "AudioStatus.h"

// Actual decode/audio generation logic
#include "AudioGeneratorAAC.h"
#include "AudioGeneratorFLAC.h"
#include "AudioGenerator.h"
#include "AudioGeneratorMIDI.h"
#include "AudioGeneratorMOD.h"
#include "AudioGeneratorMP3a.h"
#include "AudioGeneratorMP3.h"
#include "AudioGeneratorOpus.h"
#include "AudioGeneratorRTTTL.h"
#include "AudioGeneratorTalkie.h"
#include "AudioGeneratorWAV.h"

// Render(output) sounds
#include "AudioOutputBuffer.h"
#include "AudioOutputFilterDecimate.h"
#include "AudioOutput.h"
#include "AudioOutputI2S.h"
#include "AudioOutputI2SNoDAC.h"
#include "AudioOutputMixer.h"
#include "AudioOutputNull.h"
#include "AudioOutputSerialWAV.h"
#include "AudioOutputSPDIF.h"
#include "AudioOutputSPIFFSWAV.h"
#include "AudioOutputSTDIO.h"
#include "AudioOutputULP.h"
