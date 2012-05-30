#ifndef USERINTERFACE_H
#define USERINTERFACE_H


#include "Module.hpp"


namespace ProjectMaya {

	/**
	 * Represents the basic user itnerface.
	 */
	class UserInterface : public Module {
		public:

		protected:
			void init() override;
			void run() override;
			void cleanup() override;

		private:
			void initGLFW() const;
			void initOpenGL() const;
			void render() const;

			void renderOpenGL(const double&) const;
			void renderScene() const;
	};

}

#endif
