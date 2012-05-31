
#ifndef APP_H
#define APP_H

#include <memory>

#include "InteractionHandler.hpp"
#include "Logger.hpp"
#include "UserInterface.hpp"

#ifdef FIXME /// \todo buildsystem for openal
#include "SoundHandler.hpp"
#endif


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
			std::shared_ptr<InteractionHandler> ih;
			std::shared_ptr<Logger> lg;
			std::shared_ptr<UserInterface> ui;
#ifdef FIXME /// \todo buildsystem for openal
			std::shared_ptr<SoundHandler> sh;
#endif
	};

}

#endif
