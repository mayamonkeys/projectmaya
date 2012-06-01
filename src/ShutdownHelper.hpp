#ifndef SHUTDOWNHELPER_HPP
#define SHUTDOWNHELPER_HPP

#include <atomic>

namespace ProjectMaya {

	class ShutdownHelper {
		public:
			ShutdownHelper();
			void shutdown();
			bool shouldShutdown() const;

		private:
			std::atomic<bool> shutdownState;
	};

}

#endif

