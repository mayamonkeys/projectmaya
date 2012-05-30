#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <mutex>
#include <queue>

#include "Module.hpp"

namespace ProjectMaya {

	class InteractionHandler : public Module {
		public:
			void run() override;

			void newKeyEvent(bool pressed, int keyCode);

			bool exitRequested();

		private:
			struct KeyEvent {
				bool pressed;
				int keyCode;
			};

			std::mutex memberMutex;
			std::queue<KeyEvent> keyQueue;
			bool exit = false;
	};

}

#endif

