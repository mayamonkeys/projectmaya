#ifndef USERINTERFACE_H
#define USERINTERFACE_H


namespace ProjectMaya {

	/**
	 * Represents the basic user itnerface.
	 */
	class UserInterface {
		public:
			explicit UserInterface();
			~UserInterface() = default;

		private:
			void initGLFW();
			void initOpenGL();
			void render();

			void renderOpenGL(const double&);
			void renderScene();
	};

}

#endif
