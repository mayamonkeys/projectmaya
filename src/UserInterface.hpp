#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <memory>

#include "InteractionHandler.hpp"
#include "Logger.hpp"
#include "Module.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user interface.
	 */
	class UserInterface : public Module {
		public:
			UserInterface(std::shared_ptr<InteractionHandler> ih, std::shared_ptr<Logger> lg);

		protected:
			void init() override;
			void run() override;
			void cleanup() override;

		private:
			std::shared_ptr<InteractionHandler> ih;
			std::shared_ptr<Logger> lg;

			void initGLFW();
			void initOpenGL() const;
			void render() const;

			void renderOpenGL(const double&) const;
			void renderScene() const;

			void keyCallback(int id, int state);
	};

}

#endif
