#ifndef APP_H
#define APP_H

#include <memory>

#include "InteractionHandler.hpp"
#include "Logger.hpp"
#include "Module.hpp"
#include "UserInterface.hpp"


namespace ProjectMaya {

	/**
	 * Basic app, consisting of modules.
	 */
	class App {
		public:
			/// Starts all modules.
			App();
			/// Stops all modules.
			~App();
			App& operator=(const App& obj) = delete;
			void waitForShutdown() const;

		private:
			std::shared_ptr<Module<InteractionHandler>> mInteractionHandler;
			std::shared_ptr<Module<Logger>> mLogger;
			std::shared_ptr<Module<UserInterface>> mUserInterface;
	};

}

#endif
