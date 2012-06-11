#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <mutex>
#include <queue>

#include "System/ModulePayload.hpp"

namespace ProjectMaya {

	class InteractionHandler : public ModulePayload {
		public:
			InteractionHandler();
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;
	};

}

#endif
