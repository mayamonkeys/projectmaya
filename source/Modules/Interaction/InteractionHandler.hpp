#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H

#include <mutex>
#include <queue>

#include "Modules/Log/Logger.hpp"
#include "System/Module.hpp"
#include "System/ModulePayload.hpp"

namespace ProjectMaya {

	class InteractionHandler : public ModulePayload {
		public:
			InteractionHandler(std::shared_ptr<Module> lg);
			void operator()() override;

			void newKeyEvent(bool pressed, int keyCode);

			bool exitRequested();

		private:
			struct KeyEvent {
				bool pressed;
				int keyCode;
			};

			std::shared_ptr<Module> lg;

			std::mutex memberMutex;
			std::queue<KeyEvent> keyQueue;
			bool exit = false;
	};

}

#endif
