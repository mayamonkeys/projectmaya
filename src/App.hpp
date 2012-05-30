#ifndef APP_H
#define APP_H

#include <memory>

#include "UserInterface.hpp"

using std::shared_ptr;

namespace ProjectMaya {
	class App {
		public:
			App();
			App& operator=(const App& obj) = delete;
			void waitForShutdown();

		private:
			shared_ptr<UserInterface> ui;
	};
}

#endif

