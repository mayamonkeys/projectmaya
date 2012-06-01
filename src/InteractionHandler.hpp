#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <mutex>
#include <queue>

#include "Logger.hpp"
#include "Module.hpp"
#include "ShutdownHelper.hpp"

namespace ProjectMaya {

	class InteractionHandler : public ShutdownHelper {
		public:
			InteractionHandler(std::shared_ptr<Module<Logger>> lg);
			void operator()();

			void newKeyEvent(bool pressed, int keyCode);

			bool exitRequested();

		private:
			struct KeyEvent {
				bool pressed;
				int keyCode;
			};

			std::shared_ptr<Module<Logger>> lg;

			std::mutex memberMutex;
			std::queue<KeyEvent> keyQueue;
			bool exit = false;
	};

}

#endif
