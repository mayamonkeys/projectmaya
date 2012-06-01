#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <memory>

#include "InteractionHandler.hpp"
#include "Logger.hpp"
#include "Module.hpp"
#include "ShutdownHelper.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user interface.
	 */
	class UserInterface : public ShutdownHelper {
		public:
			UserInterface(std::shared_ptr<Module<InteractionHandler>> ih, std::shared_ptr<Module<Logger>> lg);
			~UserInterface();
			void operator()();

		private:
			std::shared_ptr<Module<InteractionHandler>> ih;
			std::shared_ptr<Module<Logger>> lg;

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
