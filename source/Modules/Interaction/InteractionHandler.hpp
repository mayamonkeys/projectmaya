#ifndef INTERACTIONHANDLER_HPP
#define INTERACTIONHANDLER_HPP

#include <mutex>
#include <queue>

#include "System/ModulePayload.hpp"

namespace ProjectMaya {

	class InteractionHandler : public ModulePayload {
		public:
			explicit InteractionHandler();
      ~InteractionHandler();
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;
	};

}

#endif
