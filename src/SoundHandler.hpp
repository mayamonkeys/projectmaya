#ifdef FIXME /// \todo buildsystem for openal

#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include <string>

#include "AL/alc.h"

#include "Module.hpp"
#include "Logger.hpp"

namespace ProjectMaya {

	/**
	 * Basic sound handler, makes use of your ears.
	 */
	class SoundHandler : public Module {
		public:
			explicit SoundHandler(std::shared_ptr<Logger> lg);

			void init() override;
			void run() override;
			void cleanup() override;

		private:
			std::shared_ptr<Logger> lg;

			ALCcontext* context;
			ALCdevice* device;

			void checkForErrors();
			void printDevices(ALCenum which, const std::string& kind);
			void printALCInfo();
	};

}

#endif

#endif
