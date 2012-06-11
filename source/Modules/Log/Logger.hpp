#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <queue>

#include "System/ModulePayload.hpp"

namespace ProjectMaya {

	/**
	 * Simple threadsafe, queue based, logger.
	 */
	class Logger : public ModulePayload {
		public:
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;
	};

}

#endif
