#include <chrono>
#include <sstream>
#include <stdexcept>
#include <thread>

#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alext.h>

#include "MessageTypes/StringMessage.hpp"
#include "SoundHandler.hpp"


using namespace ProjectMaya;

using std::chrono::milliseconds;
using std::noskipws;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::this_thread::sleep_for;


SoundHandler::SoundHandler() {
  device = alcOpenDevice(NULL);
  context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
}

SoundHandler::~SoundHandler() {
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

void SoundHandler::operator()() {
  /* we have to check the initialization here, b/c want to emit a msg on fail */
  checkForErrors();

	/// \todo hotfix: implement real error handling
	if(device == NULL || context == NULL)
		return;

  printALCInfo();
  checkForErrors();
  playDummySound();
}

void SoundHandler::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
  ModulePayload::setupMessageDriver(messageDriver, firstTime);

  if (firstTime) {
    this->getMessageDriver()->createSlot("log");
  }
}

void SoundHandler::checkForErrors() {
  {
    ALCenum error = alcGetError(device);

    if(error != ALC_NO_ERROR)
      this->getMessageDriver()->getSlot("log")->emit(StringMessage("ALC error occured"));
    //			throw new runtime_error("ALC error occured" /* , (const char*)alcGetString(device, error) */);
  }

  {
    ALenum error = alGetError();

    if(error != AL_NO_ERROR)
      this->getMessageDriver()->getSlot("log")->emit(StringMessage("AL error occured"));
    //			throw new runtime_error("AL error occured" /* , (const char*)alGetString(error) */);
  }
}

void SoundHandler::printDevices(ALCenum which, const string& kind) {
  const char *s = alcGetString(NULL, which);
  checkForErrors();

  stringstream stream;
  stream << noskipws << "Available " << kind << "devices: ";

  while(*s != '\0') {
    stream << s << " ";

    while(*s++ != '\0')
      ;
  }

  this->getMessageDriver()->getSlot("log")->emit(StringMessage(stream.str()));
}

void SoundHandler::printALCInfo() {
  ALCint major, minor;

  stringstream stream;
  stream << noskipws;

  if(alcIsExtensionPresent(NULL, (const ALCchar*)"ALC_ENUMERATION_EXT") == AL_TRUE) {
    if(alcIsExtensionPresent(NULL, (const ALCchar*)"ALC_ENUMERATE_ALL_EXT") == AL_TRUE) {
      printDevices(ALC_ALL_DEVICES_SPECIFIER, "playback ");
    } else {
      printDevices(ALC_DEVICE_SPECIFIER, "playback ");
    }
  }	else {
    stream << "No device enumeration available";
    this->getMessageDriver()->getSlot("log")->emit(StringMessage(stream.str()));
    stream.str("");
  }

  stream << "Default device: " << alcGetString(device, ALC_DEFAULT_DEVICE_SPECIFIER);
  this->getMessageDriver()->getSlot("log")->emit(StringMessage(stream.str()));
  stream.str("");


  alcGetIntegerv(device, ALC_MAJOR_VERSION, 1, &major);
  alcGetIntegerv(device, ALC_MINOR_VERSION, 1, &minor);
  checkForErrors();

  stream << "ALC version: " << (int)major << "." << (int)minor;
  this->getMessageDriver()->getSlot("log")->emit(StringMessage(stream.str()));
}

void SoundHandler::playDummySound() {
  /* create default listener */

  // forward then up vector
  ALfloat flValues[6] = {0, 1, 0, 0, 0, 1};

  alListener3f(AL_POSITION, 0, 0, 0);
  alListener3f(AL_VELOCITY, 0, 0, 0);
  alListenerfv(AL_ORIENTATION, flValues);
  checkForErrors();

  /* create default source */
  ALuint source;
  alGenSources(1, &source);
  checkForErrors();

  alSourcef(source, AL_PITCH, 1);
  alSourcef(source, AL_GAIN, 1);
  alSource3f(source, AL_POSITION, 0, 0, 0);
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  alSourcei(source, AL_LOOPING, AL_TRUE);
  checkForErrors();

  /* create buffer for source */
  ALuint buffer;
  alGenBuffers(1, &buffer);
  checkForErrors();



  /* fill buffer */
  int sampleRate = 44100;
  int seconds = 10;
  int frames = seconds * sampleRate;
  int frequency = 1800;
  ALbyte data[frames];

  for(int i = 0; i < frames; ++i)
    data[i] = 25;

  alBufferData(buffer, AL_FORMAT_MONO8, &data, sizeof(data), frequency);



  /* attach and play */
  alSourcei(source, AL_BUFFER, buffer);
  alSourcePlay(source);



  this->getMessageDriver()->getSlot("log")->emit(StringMessage("playing noise"));
  milliseconds stime(2000);
  sleep_for(stime);
  this->getMessageDriver()->getSlot("log")->emit(StringMessage("stopping noise"));



  /* stop and cleanup */
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  checkForErrors();
}

