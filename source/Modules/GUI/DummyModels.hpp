#ifndef DUMMYOBJECTS_HPP
#define DUMMYOBJECTS_HPP

#include <GL/gl.h>


namespace ProjectMaya {

	/**
	 * Data for a const Dummy Box.
	 * You probably have to scale the values to your needs.
	 */
	namespace Quad {
		static const GLfloat edges[] = {
			-1.0, -1.0, 0.0,
			1.0, -1.0, 0.0,
			1.0, 1.0, 0.0,
			-1.0, 1.0, 0.0
		};

		static const GLfloat texture[] = {
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0
		}
	};

}

#endif
