#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <memory>

#include "System/ModulePayload.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user interface.
	 */
	class UserInterface : public ModulePayload {
		public:
			explicit UserInterface();
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
			void charCallback(int codepoint, int state);

			void queryVideoModes();
	};

}

#endif
