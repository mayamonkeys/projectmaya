#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <memory>

#include "Modules/Interaction/InteractionHandler.hpp"
#include "System/Module.hpp"
#include "System/ModulePayload.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user interface.
	 */
	class UserInterface : public ModulePayload {
		public:
			UserInterface(std::shared_ptr<Module> ih, std::shared_ptr<Module> lg);
			~UserInterface();
			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime);

		private:
			std::shared_ptr<Module> ih;
			std::shared_ptr<Module> lg;

			void initGLFW();
			void initOpenGL() const;
			void render() const;

			void renderOpenGL(const double&) const;
			void renderScene() const;

			void keyCallback(int id, int state);

			void queryVideoModes();
	};

}

#endif
