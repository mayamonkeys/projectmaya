#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <memory>

#include "System/ModulePayload.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user interface.
	 */
	class UserInterface : public ModulePayload {
		public:
			UserInterface();
			~UserInterface();
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime);

		private:
			void initGLFW();
			void initOpenGL() const;
			void render();

			void renderOpenGL(const double&) const;
			void renderScene() const;

			void keyCallback(int id, int state);

			void queryVideoModes();
	};

}

#endif
