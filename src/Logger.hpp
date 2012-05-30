#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <queue>

#include "Module.hpp"

namespace ProjectMaya {

	/**
	 * Simple threadsafe, queue based, logger.
	 */
	class Logger : public Module {
		public:
			void run() override;
			bool exitRequested();

			/// Formats and logs the provided message.
			void log(const std::string msg);
			/// Formats and logs the specified id and the provided message.
			void log(int id, const std::string msg);
			/// Formats and logs the name of the caller and the provided message.
			void log(const std::string name, const std::string msg);

		private:
			std::mutex memberMutex;
			std::queue<const std::string> msgQueue;
	};

}

#endif
