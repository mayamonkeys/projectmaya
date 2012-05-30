#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <thread>

#include "Module.hpp"

using std::thread;

namespace ProjectMaya {

	/**
	 * Represents the basic user itnerface.
	 */
	class UserInterface : public Module {
		public:
			explicit UserInterface();
			~UserInterface() = default;

		protected:
			void run();

		private:
			void initGLFW();
			void initOpenGL();
			void render();

			void renderOpenGL(const double&);
			void renderScene();
	};

}

#endif
