#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <mutex>
#include <queue>

#include "System/Module.hpp"
#include "System/ModulePayload.hpp"

namespace ProjectMaya {

	class InteractionHandler : public ModulePayload {
		public:
			InteractionHandler(std::shared_ptr<Module> lg);
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;

		private:
			std::shared_ptr<Module> lg;

			std::mutex memberMutex;
	};

}

#endif
