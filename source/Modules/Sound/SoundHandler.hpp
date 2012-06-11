#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include <string>

#include <AL/alc.h>

#include "System/Module.hpp"
#include "System/ModulePayload.hpp"

namespace ProjectMaya {

	/**
	 * Basic sound handler, makes use of your ears.
	 */
	class SoundHandler : public ModulePayload {
		public:
			explicit SoundHandler(std::shared_ptr<Module> lg);

			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;

		private:
			std::shared_ptr<Module> lg;

			ALCcontext* context;
			ALCdevice* device;

			void checkForErrors();
			void printDevices(ALCenum which, const std::string& kind);
			void printALCInfo();
	};

}

#endif

