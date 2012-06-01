#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <memory>

#include "InteractionHandler.hpp"
#include "Logger.hpp"
#include "Module.hpp"
#include "ModulePayload.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user interface.
	 */
	class UserInterface : public ModulePayload {
		public:
			UserInterface(std::shared_ptr<Module> ih, std::shared_ptr<Module> lg);
			~UserInterface();
			void operator()() override;

		private:
			std::shared_ptr<Module> ih;
			std::shared_ptr<Module> lg;

			void initGLFW();
			void initOpenGL() const;
			void render() const;

			void renderOpenGL(const double&) const;
			void renderScene() const;

			void keyCallback(int id, int state);

			void queryVideoModes() const;
	};

}

#endif
