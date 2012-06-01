#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <queue>

#include "ShutdownHelper.hpp"

namespace ProjectMaya {

	/**
	 * Simple threadsafe, queue based, logger.
	 */
	class Logger : public ShutdownHelper {
		public:
			void operator()();

			/// Formats and logs the provided message.
			void log(const std::string& msg);
			/// Formats and logs the specified id and the provided message.
			void log(const int& id, const std::string& msg);
			/// Formats and logs the name of the caller and the provided message.
			void log(const std::string& name, const std::string& msg);
			/// Formats and logs the provided message.
			Logger& operator<<(const std::string& msg) { log(msg); return *this; }

		private:
			std::mutex memberMutex;
			std::queue<const std::string> msgQueue;
	};

}

#endif
