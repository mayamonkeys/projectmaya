#ifndef APP_H
#define APP_H

#include <memory>

#include "System/Module.hpp"

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
			std::shared_ptr<Module> mInteractionHandler;
			std::shared_ptr<Module> mInterpreter;
			std::shared_ptr<Module> mLogger;
			std::shared_ptr<Module> mSoundHandler;
			std::shared_ptr<Module> mUserInterface;
	};

}

#endif
