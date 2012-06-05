#ifndef MODULEPAYLOAD_HPP
#define MODULEPAYLOAD_HPP

#include <atomic>

namespace ProjectMaya {

	class ModulePayload {
		public:
			ModulePayload();
			virtual ~ModulePayload();
			virtual void operator()();
			virtual void shutdown();
			bool shouldShutdown() const;

		private:
			std::atomic<bool> shutdownState;
	};

}

#endif
