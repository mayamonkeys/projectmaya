#ifndef MODULEPAYLOAD_HPP
#define MODULEPAYLOAD_HPP

#include <atomic>
#include <memory>

#include "MessageDriver.hpp"

namespace ProjectMaya {

	class ModulePayload {
		public:
			ModulePayload();
			virtual ~ModulePayload();
			virtual void operator()();
			virtual void shutdown();
			virtual void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime);

		protected:
			bool shouldShutdown() const;
			std::shared_ptr<MessageDriver> getMessageDriver();

		private:
			std::atomic<bool> shutdownState;
			std::shared_ptr<MessageDriver> messageDriver;
	};

}

#endif
