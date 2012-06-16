#ifndef LOGGER_HPP
#define LOGGER_HPP

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
      explicit Logger();
      ~Logger();
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;
	};

}

#endif
