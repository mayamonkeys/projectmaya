#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <mutex>
#include <queue>

#include "Module.hpp"
#include "Logger.hpp"

namespace ProjectMaya {

	class InteractionHandler : public Module {
		public:
			InteractionHandler(std::shared_ptr<Logger> lg);
			void run() override;

			void newKeyEvent(bool pressed, int keyCode);

			bool exitRequested();

		private:
			struct KeyEvent {
				bool pressed;
				int keyCode;
			};

			std::shared_ptr<Logger> lg;

			std::mutex memberMutex;
			std::queue<KeyEvent> keyQueue;
			bool exit = false;
	};

}

#endif
