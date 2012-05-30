#ifndef APP_H
#define APP_H

#include <memory>

#include "UserInterface.hpp"


namespace ProjectMaya {

	/**
	 * Basic app, consisting of modules.
	 */
	class App {
		public:
			App();
			App& operator=(const App& obj) = delete;
			void waitForShutdown() const;

		private:
			std::shared_ptr<UserInterface> ui;
	};

}

#endif
