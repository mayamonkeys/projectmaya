#include <sstream>
#include <stdexcept>

#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alext.h>

#include "Logger.hpp"
#include "SoundHandler.hpp"


using namespace ProjectMaya;

using std::noskipws;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::stringstream;


SoundHandler::SoundHandler(shared_ptr<Module> lg) {
	this->lg = lg;
}

void SoundHandler::operator()() {
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	checkForErrors();

	printALCInfo();
	checkForErrors();

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void SoundHandler::checkForErrors() {
	{
		ALCenum error = alcGetError(device);

		if(error != ALC_NO_ERROR)
			throw new runtime_error("ALC error occured" /* , (const char*)alcGetString(device, error) */);
	}

	{
		ALenum error = alGetError();

		if(error != AL_NO_ERROR)
			throw new runtime_error("AL error occured" /* , (const char*)alGetString(error) */);
	}
}

void SoundHandler::printDevices(ALCenum which, const string& kind) {
	const char *s = alcGetString(NULL, which);
	checkForErrors();

	stringstream stream;
	stream << noskipws << "Available " << kind << "devices: ";

	while(*s != '\0')	{
		stream << s << " ";

		while(*s++ != '\0')
			;
	}

	lg->get<Logger>().log("SoundHandler", stream.str());
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
		lg->get<Logger>().log("SoundHandler", stream.str());
		stream << "";
	}

	stream << "Default device: " << alcGetString(device, ALC_DEFAULT_DEVICE_SPECIFIER);
	lg->get<Logger>().log("SoundHandler", stream.str());
	stream << "";


	alcGetIntegerv(device, ALC_MAJOR_VERSION, 1, &major);
	alcGetIntegerv(device, ALC_MINOR_VERSION, 1, &minor);
	checkForErrors();

	stream << "ALC version: " << (int)major << "." << (int)minor;
	lg->get<Logger>().log("SoundHandler", stream.str());
}

